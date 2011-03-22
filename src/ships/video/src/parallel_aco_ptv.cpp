#include <ships/video/video.hpp>

namespace Sp {

void ParallelACOPTVEstimator::Loop() {

}

void ParallelACOPTVEstimator::Cluster() {
	int np = this->num_particles_;
	this->clusters0_ = new Matrix<int, int> (np, this->cluster_max_);
	this->clusters1_ = new Matrix<int, int> (np, this->cluster_max_);
	this->clusters01_ = new Matrix<int, int> (np, this->cluster01_max_);
	//initialize the index matrices.
	for (int i = 0; i < np; i++) //for each row
		for (int j = 0; j < np; j++) { //for each element in a row
			clusters0_->Set(j, i, j);
			clusters1_->Set(j, i, j);
			clusters01_->Set(j, i, j);
		}

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
		result = new Matrix<float,float>(f0->GetHeight(), f1->GetHeight());
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
	int err=0;

	if(matrix->IsMapped())
		matrix->GetArray().UnMap();

	if(indices->IsMapped())
		indices->GetArray().UnMap();

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.clusters");
	if (kernel == NULL)
		throw InvalidKernelException();

	err = kernel->setArg(0,matrix->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);
	err = kernel->setArg(1,indices->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 1: "<<err);

	err = kernel->setArg(2,this->cluster_size_*sizeof(int)*this->num_particles_,NULL);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 2: "<<err);

	err = kernel->setArg(3,indices->GetWidth());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);

	err = kernel->setArg(4,this->num_particles_);
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 4: "<<err);

	cl::NDRange global(this->num_particles_);
	cl::NDRange local(this->num_particles_/16);

	err = queue_.enqueueNDRangeKernel(*kernel, cl::NullRange, global, local);

	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

	queue_.finish();

	if (!matrix->IsMapped())
		matrix->GetArray().Map();
	if (!indices->IsMapped())
		indices->GetArray().Map();

/*
	if(indices->IsMapped())
		indices->GetArray().UnMap();

	cl::Kernel* kernel = DeviceManager::Instance().FindKernel(
			"ships.video.aco_ptv.zeros");

	err = kernel->setArg(0,indices->GetArray().GetMemoryModel().GetImage2D());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 0: "<<err);

	err = kernel->setArg(1,indices->GetWidth());
	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(video_logger,"error in setting kernel arg 3: "<<err);

	cl::NDRange global(this->num_particles_);
	cl::NDRange local(this->num_particles_/16);

	err = queue_.enqueueNDRangeKernel(*kernel, cl::NullRange, global, local);

	if (err != CL_SUCCESS)
		LOG4CXX_ERROR(Sp::video_logger, "error in running kernel:"<<err);

	if (!indices->IsMapped())
		indices->GetArray().Map();
*/
}

}
