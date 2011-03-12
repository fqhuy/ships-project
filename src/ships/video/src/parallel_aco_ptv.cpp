#include <ships/video/video.hpp>

namespace Sp{

void ParallelACOPTVEstimator::Loop(){

}

void ParallelACOPTVEstimator::Cluster(){

}

Matrix<float, float>* ParallelACOPTVEstimator::Distances(Matrix<float, float>* f0, Matrix<float,
		float>* f1, bool relaxation){
	if(f0->IsMapped())
		f0->GetArray().GetMemoryModel().UnMap();
	if(f1->IsMapped())
		f1->GetArray().GetMemoryModel().UnMap();

	cl::Kernel* kl = DeviceManager::Instance().FindKernel("ships.video.aco_ptv.distances");
	if(kl==NULL)
		throw InvalidKernelException();
	MemoryModel<float>* mm = new MemoryModel<float>(1,false,true,1,READ_WRITE);
	uint32_t dims[] = {f0->GetHeight(), f1->GetHeight()};
	SampleModel<float,float>* sm = new PixelInterleavedSampleModel<float>(1,2,dims);
	Matrix<float,float>* result = new Matrix<float,float>(f0->GetHeight(),f1->GetHeight(),mm,sm);

	kl->setArg(0,result->GetArray().GetMemoryModel().GetBuffer());
	kl->setArg(1,f0->GetArray().GetMemoryModel().GetBuffer());
	kl->setArg(2,f1->GetArray().GetMemoryModel().GetBuffer());
    cl::NDRange global();
    cl::NDRange local();
   // queue_.enqueueNDRangeKernel(*kl, cl::NullRange, global, local);
}

void ParallelACOPTVEstimator::AddFrame(Matrix<float,float>* frame) throw (InvalidContextException){
	if(this->frames_.size()==0){
		if(frame->GetArray().GetMemoryModel().GetContext()()==NULL)
			throw InvalidContextException();
		this->context_ = frame->GetArray().GetMemoryModel().GetContext();
		int err;
		std::vector<cl::Device> devices  = this->context_.getInfo<CL_CONTEXT_DEVICES>(&err);
		if(err!=CL_SUCCESS)
			throw InvalidContextException();
		//currently, only one device is supported
		this->queue_ = cl::CommandQueue(this->context_,devices[0]);
	}

	if(frame->GetArray().GetMemoryModel().GetContext()()!=this->context_()){
		throw InvalidContextException();
	}
	this->frames_.push_back(frame);
}

}
