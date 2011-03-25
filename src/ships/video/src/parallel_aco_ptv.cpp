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

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.one_step");
	cl::Kernel* kernel1 = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.update");

	if (kernel == NULL)
		throw new InvalidKernelException();
	if (kernel1 == NULL)
		throw new InvalidKernelException();

	int gx = this->num_particles_;
	int gy = this->num_threads_ / this->num_particles_;

	int lx = gx / this->num_blocks_;
	int ly = gy;

	cl::NDRange global(gx, gy);
	cl::NDRange local(lx, ly);

	for (i = 0; i < nl; i++) { //for nl number of loops.
		rd.SRand();

		if (this->cluster_size_ < this->cluster_max_)
			this->cluster_size_++;

		Vector<int, int> initPos(na);
		Vector<int, int> Tk0(np);
		Vector<int, int> Tk1(np);
		Vector<float, float> Lk(na);

		//generate random init positions.
		rd.NextN(initPos.GetArray().GetData(), na, 0, np, false);

		//init start positions for each ant.
		for (j = 0; j < na; j++) {
			for (k = 0; k < np; k++) {
				Tk0.Set((k + this->num_particles_ - initPos.Get(j))
						% this->num_particles_, j * np + k);
				Tk1.Set(0, j * np + k);
			}
		}
		initPos.GetArray().UnMap();
		Tk0.GetArray().UnMap();
		Tk1.GetArray().UnMap();
		Lk.GetArray().UnMap();

		{//run one_step kernel
			err = kernel->setArg(0,
					this->clusters0_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
			err = kernel->setArg(1,
					this->clusters1_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);
			err
					= kernel->setArg(
							2,
							this->clusters01_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);
			err = kernel->setArg(3,
					this->cf0f1_->GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);
			err = kernel->setArg(4,
					this->f0_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);
			err = kernel->setArg(5,
					this->f1_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 5: "<<err);
			err = kernel->setArg(6,
					this->tau_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 6: "<<err);
			err = kernel->setArg(7, Lk.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 7: "<<err);
			err = kernel->setArg(8,
					Tk0.GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 8: "<<err);
			err = kernel->setArg(9,
					Tk1.GetArray().GetMemoryModel().GetImage2D());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 9: "<<err);
			err = kernel->setArg(10,
					initPos.GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 10: "<<err);
			err = kernel->setArg(11,
					this->N_->GetArray().GetMemoryModel().GetBuffer());
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 11: "<<err);
			err = kernel->setArg(12, this->num_particles_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 12: "<<err);
			err = kernel->setArg(13, this->num_ants_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 13: "<<err);
			err = kernel->setArg(14, this->cluster01_max_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 14: "<<err);
			err = kernel->setArg(15, this->cluster_max_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 15: "<<err);
			err = kernel->setArg(16, this->cluster_size_);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(video_logger,"error in setting kernel arg 16: "<<err);

			err = this->queue_.enqueueNDRangeKernel(*kernel, cl::NullRange,
					global, local);
			if (err != CL_SUCCESS)
				LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

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

			Tk0.GetArray().GetMemoryModel().Map();
			Tk1.GetArray().GetMemoryModel().Map();
			Lk.GetArray().GetMemoryModel().Map();

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
				this->result_->Set(Tk0.Get(ibestLk * np + j), k, 0);//(rf0[i], i, 0);
				this->result_->Set(Tk1.Get(ibestLk * np + j), k, 1);//(rf1[i], i, 1);
			}
		}
	}

}

void ParallelACOPTVEstimator::Cluster() {
	int np = this->num_particles_;
	this->clusters0_ = new Matrix<int, int> (np, this->cluster_max_);
	this->clusters1_ = new Matrix<int, int> (np, this->cluster_max_);
	this->clusters01_ = new Matrix<int, int> (np, this->cluster01_max_);

	this->Sort(this->cf0_, this->clusters0_);
	this->Sort(this->cf1_, this->clusters1_);
	this->Sort(this->cf0f1_, this->clusters01_);
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
		result = new Matrix<float, float> (f0->GetHeight(), f1->GetHeight());
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

	if (!f0->IsMapped())
		f0->GetArray().Map();
	if (!f1->IsMapped())
		f1->GetArray().Map();

	result->GetArray().Map();

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

	if (!matrix->IsMapped())
		matrix->GetArray().Map();
	if (!indices->IsMapped())
		indices->GetArray().Map();
}

}
