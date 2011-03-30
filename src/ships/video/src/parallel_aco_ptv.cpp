#include <ships/video/video.hpp>

namespace Sp {

void ParallelACOPTVEstimator::Loop() {
	Random rd;
	int i, j, k, h, next, err = 0;
	float tau, delta_tau, l, Lk, bestLk = INT_MAX, lbestLk;
	//final result
	int *rf0, *rf1, *lrf0, *lrf1, in = 1;

	int np = this->num_particles_;
	int na = this->num_ants_;
	int nl = this->num_loops_;

	HostVector<int, int> initPos(na);

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.one_step");
	cl::Kernel* kernel1 = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.update");
	cl::Kernel* kernel2 = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.update_rho");

	if (kernel == NULL)
		throw InvalidKernelException();
	if (kernel1 == NULL)
		throw InvalidKernelException();
	if(kernel2 == NULL)
		throw InvalidKernelException();

	int gx = this->num_particles_;
	int	gy =(this->num_particles_ * this->num_ants_) <= this->num_threads_ ? this->num_ants_ : this->num_threads_ / this->num_particles_;

	int lx = gx / this->num_blocks_;
	int ly = gy;

	cl::NDRange global(gx, gy);
	cl::NDRange local(lx, ly);

	this->tau_->GetArray().UnMap();
	//Lk.GetArray().UnMap();
	this->N_->GetArray().UnMap();
	this->f0_->GetArray().UnMap();
	this->f1_->GetArray().UnMap();

	for (i = 0; i < nl; i++) { //for nl number of loops.
		//rd.SRand();

		//if (this->cluster_size_ < this->cluster_max_)
		//	this->cluster_size_++;

		Vector<int, int> Tk0(np * na);
		Vector<int, int> Tk1(np * na);
		Vector<float, float> Lk(na);

		//generate random init positions.
		rd.NextN(initPos.GetArray().GetData(), na, 0, np, false);

		//init start positions for each ant.
		for (j = 0; j < na; j++) {
			Lk.Set(2, j);
			for (k = 0; k < np; k++) {
				Tk0.Set((k + this->num_particles_ - initPos.Get(j))
						% this->num_particles_, j * np + k);
				Tk1.Set(0, j * np + k);
			}
		}

		//LOG4CXX_INFO(Sp::video_logger,Lk.ToString());
		this->Init(this->N_,"init_bufferi",1,this->num_particles_,this->num_ants_);
		Lk.GetArray().UnMap();
		//initPos.GetArray().UnMap();
		Tk0.GetArray().UnMap();
		Tk1.GetArray().UnMap();

		{//run one_step kernel
			err = kernel->setArg(0,
					this->cf0f1_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
			err
					= kernel->setArg(
							1,
							this->clusters01_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);
			err = kernel->setArg(2,
					this->tau_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);
			err = kernel->setArg(3, Lk.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);
			err
					= kernel->setArg(4,
							Tk0.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);
			err
					= kernel->setArg(5,
							Tk1.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 5: "<<err);
			err = kernel->setArg(6,
					this->N_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 6: "<<err);
			err = kernel->setArg(7, this->num_particles_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 7: "<<err);
			err = kernel->setArg(8, this->num_ants_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 8: "<<err);
			err = kernel->setArg(9, this->cluster01_max_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 9: "<<err);
			err = kernel->setArg(10, this->cluster_max_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 10: "<<err);
			err = kernel->setArg(11, this->cluster_size_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 11: "<<err);

			err = this->queue_.enqueueNDRangeKernel(*kernel, cl::NullRange,
					global, local);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

			this->queue_.finish();

		}
		{//update tau matrix

			err = kernel2->setArg(0,
					this->tau_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<2> arg 0: "<<err);
			err = kernel2->setArg(1, this->num_particles_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<2> arg 1: "<<err);

			err = this->queue_.enqueueNDRangeKernel(*kernel2, cl::NullRange,
					cl::NDRange(this->num_particles_, this->num_threads_/ this->num_particles_),
					cl::NDRange(this->num_particles_ / this->num_blocks_, this->num_threads_ / this->num_particles_));
			if(err!=CL_SUCCESS)
			{
				LOG4CXX_ERROR(Sp::video_logger,"an error occurred when running update_rho kernel");
				return;
			}
			this->queue_.finish();

		}
		{//run update kernel

			err = kernel1->setArg(0,
					this->tau_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 0: "<<err);
			err
					= kernel1->setArg(1,
							Lk.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 1: "<<err);
			err = kernel1->setArg(2,
					Tk0.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 2: "<<err);
			err = kernel1->setArg(3,
					Tk1.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 3: "<<err);
			err = kernel1->setArg(4, this->num_ants_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 4: "<<err);
			err = kernel1->setArg(5, this->num_particles_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel<1> arg 5: "<<err);

			err = this->queue_.enqueueNDRangeKernel(*kernel1, cl::NullRange,
					global, local);

			this->queue_.finish();

		}

		if (i == (nl - 1)) {

			Tk0.GetArray().Map();
			Tk1.GetArray().Map();
			Lk.GetArray().Map();

			//LOG4CXX_INFO(Sp::video_logger,"Lk: \n"<<Lk.ToString());
			//walk through the Lk array to find the smallest total distance.
			this->result_ = new HostMatrix<int, int> (2, np);
			bestLk = INT_MAX;
			int ibestLk;

			for (j = 0; j < na; j++) {
				if (bestLk > Lk.Get(j)) {
					bestLk = Lk.Get(j);
					ibestLk = j;
				}
			}
			for (j = 0; j < np; j++) {
				this->result_->Set(Tk0.Get(ibestLk * np + j), j, 0);//(rf0[i], i, 0);
				this->result_->Set(Tk1.Get(ibestLk * np + j), j, 1);//(rf1[i], i, 1);
			}
		}
	}

}

void ParallelACOPTVEstimator::Cluster() {
	int np = this->num_particles_;
	this->clusters0_ = new DeviceMatrix<int, int> (this->cluster_max_, np);
	this->clusters1_ = new DeviceMatrix<int, int> (this->cluster_max_, np);
	this->clusters01_ = new DeviceMatrix<int, int> (this->cluster01_max_, np);

	this->Sort(this->cf0_, this->clusters0_);
	this->Sort(this->cf1_, this->clusters1_);
	this->Sort(this->cf0f1_, this->clusters01_);

	//DeviceMatrix<float,float> result(this->cluster01_max_,this->num_particles_);
	delete this->cf0f1_; //reuse cf0f1;
	this->cf0f1_ = this->RDistances();
	//this->new_clusters01_ = new DeviceMatrix<int,int>(this->cluster01_max_,this->num_particles_);
	//this->new_cf0f1_ = new DeviceMatrix<float,float>(this->cluster01_max_,this->num_particles_);

	//this->new_cf0f1_->GetArray().UnMap();
	//this->new_clusters01_->GetArray().UnMap();

	//this->RSort(this->cf0f1_,this->clusters01_,this->new_cf0f1_,this->new_clusters01_);

	//this->new_cf0f1_->GetArray().Map();
	//this->new_clusters01_->GetArray().Map();

	//LOG4CXX_INFO(Sp::video_logger,this->new_clusters01_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,this->new_cf0f1_->ToString());
}

Matrix<float, float>* ParallelACOPTVEstimator::RDistances() {
	int err;
	cl::Kernel* kl = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.r_distances");
	if (!kl)
		return NULL;
	//TODO: remmember to change this back to DeviceMatrix !!!!!
	Matrix<float, float> *result = new DeviceMatrix<float, float> (
			this->cluster01_max_, this->num_particles_);
	//result->GetArray().UnMap();

	err = kl->setArg(0, result->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
	err = kl->setArg(1,
			this->clusters0_->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);
	err = kl->setArg(2,
			this->clusters1_->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);
	err = kl->setArg(3,
			this->clusters01_->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);
	err = kl->setArg(4, this->f0_->GetArray().GetMemoryModel().GetBuffer());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);
	err = kl->setArg(5, this->f1_->GetArray().GetMemoryModel().GetBuffer());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 5: "<<err);
	err = kl->setArg(6, this->num_particles_);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 6: "<<err);
	err = kl->setArg(7, this->cluster_size_);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 7: "<<err);

	cl::NDRange global(this->cluster01_max_ * this->num_blocks_,
			this->num_threads_ / (this->cluster01_max_ * this->num_blocks_));
	cl::NDRange local(this->cluster01_max_, this->num_threads_
			/ (this->cluster01_max_ * this->num_blocks_));

	err = queue_.enqueueNDRangeKernel(*kl, cl::NullRange, global, local);
	this->queue_.finish();
	if (err != CL_SUCCESS)
		return NULL;

	//LOG4CXX_INFO(Sp::video_logger,result->ToString());
	//result->GetArray().Map();
	return result;
}

Matrix<float, float>* ParallelACOPTVEstimator::Distances(
		Matrix<float, float>* f0, Matrix<float, float>* f1,
		Matrix<float, float>* result, bool relaxation) {
	int err = 0;

	cl::Kernel* kl = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.distances");
	if (kl == NULL)
		throw InvalidKernelException();

	if (result == NULL) {
		result = new DeviceMatrix<float, float> (f0->GetHeight(),
				f1->GetHeight());
		/*
		 MemoryModel<float>* mm = new MemoryModel<float> (2, false, true, 1,
		 READ_WRITE);
		 uint32_t dims[] = { f0->GetHeight(), f1->GetHeight() };
		 SampleModel<float, float>* sm =
		 new PixelInterleavedSampleModel<float> (1, 2, dims);
		 result = new Matrix<float, float> (f0->GetHeight(), f1->GetHeight(),
		 mm, sm);
		 */
	}

	if (f0->IsMapped())
		f0->GetArray().UnMap();
	if (f1->IsMapped())
		f1->GetArray().UnMap();
	if (result->IsMapped())
		result->GetArray().UnMap();

	err = kl->setArg(0, result->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);

	err = kl->setArg(1, f0->GetArray().GetMemoryModel().GetBuffer());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);
	err = kl->setArg(2, f1->GetArray().GetMemoryModel().GetBuffer());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel: arg 2"<<err);
	err = kl->setArg(3, f0->GetHeight());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel: arg 3"<<err);

	int gx = f0->GetHeight();
	int gy = 4096 / gx;

	int lx = gx / 32;
	int ly = gy;

	cl::NDRange global(gx, gy);
	cl::NDRange local(lx, ly);

	err = queue_.enqueueNDRangeKernel(*kl, cl::NullRange, global, local);
	queue_.finish();

	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);
	/*
	 if (!f0->IsMapped())
	 f0->GetArray().Map();
	 if (!f1->IsMapped())
	 f1->GetArray().Map();

	 result->GetArray().Map();
	 */
	return result;
}

void ParallelACOPTVEstimator::AddFrame(Matrix<float, float>* frame)
		throw (InvalidContextException) {
	if (this->frames_.size() == 0) {
		if (frame->GetArray().GetMemoryModel().GetContext()() == NULL)
			throw InvalidContextException();
		this->context_ = frame->GetArray().GetMemoryModel().GetContext();
		int err;
		std::vector<cl::Device> devices = this->context_.getInfo<
				CL_CONTEXT_DEVICES> (&err);
		if (err != CL_SUCCESS)
			throw InvalidContextException();
		//currently, only one device is supported
		this->queue_ = cl::CommandQueue(this->context_, devices[0]);
	}

	if (frame->GetArray().GetMemoryModel().GetContext()() != this->context_()) {
		throw InvalidContextException();
	}
	this->frames_.push_back(frame);
}

float ParallelACOPTVEstimator::RelaxationLength(const int& p0, const int& p1) {

}

void ParallelACOPTVEstimator::RSort(Matrix<float, float>* dis,
		Matrix<int, int>* ids, Matrix<float, float>* new_dis,
		Matrix<int, int>* new_ids) {
	int err = 0;

	if (dis->IsMapped())
		dis->GetArray().UnMap();

	if (ids->IsMapped())
		ids->GetArray().UnMap();

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.sort");

	if (kernel == NULL)
		throw InvalidKernelException();

	err = kernel->setArg(0, dis->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
	err = kernel->setArg(1, ids->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);

	err = kernel->setArg(2, new_dis->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);

	err = kernel->setArg(3, new_ids->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);

	err = kernel->setArg(4, this->cluster01_max_);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);

	//cl::NDRange global(this->cluster01_max_*this->num_blocks_,this->num_threads_/(this->cluster01_max_ * this->num_blocks_));
	//cl::NDRange local(this->cluster01_max_,this->num_threads_/(this->cluster01_max_ * this->num_blocks_));

	err = queue_.enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(
			this->num_particles_), cl::NDRange(this->num_particles_
			/ this->num_blocks_));

	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

	queue_.finish();
	/*
	 if (!dis->IsMapped())
	 dis->GetArray().Map();
	 if (!indices->IsMapped())
	 indices->GetArray().Map();
	 */
}

void ParallelACOPTVEstimator::Sort(Matrix<float, float>* matrix, Matrix<int,
		int>* indices) {
	int err = 0;

	if (matrix->IsMapped())
		matrix->GetArray().UnMap();

	if (indices->IsMapped())
		indices->GetArray().UnMap();

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.clusters");
	if (kernel == NULL)
		throw InvalidKernelException();

	err = kernel->setArg(0, matrix->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
	err = kernel->setArg(1, indices->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);

	err = kernel->setArg(2, this->cluster_size_ * sizeof(int)
			* this->num_particles_, NULL);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);

	err = kernel->setArg(3, indices->GetWidth());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);

	err = kernel->setArg(4, this->num_particles_);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);

	cl::NDRange global(this->num_particles_);
	cl::NDRange local(this->num_particles_ / 16);

	err = queue_.enqueueNDRangeKernel(*kernel, cl::NullRange, global, local);

	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

	queue_.finish();
	/*
	 if (!matrix->IsMapped())
	 matrix->GetArray().Map();
	 if (!indices->IsMapped())
	 indices->GetArray().Map();
	 */
}
template<class E, class T> void ParallelACOPTVEstimator::Init(T* container, std::string kernel_name, E value, const int& size1, const int& size2){
	int err=0;
	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(kernel_name);
	if(kernel==NULL)
		return;

	int num_dims = container->GetArray().GetMemoryModel().GetNumDims();

	if(num_dims==2)
		err = kernel->setArg(0,container->GetArray().GetMemoryModel().GetImage2D());
	else if(num_dims==1)
		err = kernel->setArg(0,container->GetArray().GetMemoryModel().GetBuffer());

	err &= kernel->setArg(1, value);

	err &= kernel->setArg(2, size1);
	err &= kernel->setArg(3, size2);


	err &= this->queue_.enqueueNDRangeKernel(*kernel,cl::NullRange,cl::NDRange(size1,this->num_threads_/size1),cl::NDRange(size1/this->num_blocks_,this->num_threads_/size1));

	if(err!=CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "an error occurred in initializing with kernel"<< kernel_name);
}
}
