/*
 * model.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: fqhuy
 */

#include <ships/core/core.hpp>

namespace Sp {

#define INSTANTIATE_MEMORY_MODEL(T) \
	template class MemoryModel<T>; \
	template MemoryModel<T>* MemoryModel<T>::Clone(); \
	template T* MemoryModel<T>::CreateArray(size_t width); \
	template T* MemoryModel<T>::CreateArray(uint32_t width, uint32_t height); \
	template T* MemoryModel<T>::Map(int32_t x, int32_t y, uint32_t width, uint32_t height); \
	template T* MemoryModel<T>::Map(int32_t offset, size_t size); \
	template T* MemoryModel<T>::Map(); \
	template void MemoryModel<T>::UnMap();

#define INSTANTIATE_HOST_MEMORY_MODEL(T) \
	template class HostMemoryModel<T>; \
	template MemoryModel<T>* HostMemoryModel<T>::Clone(); \
	template T* HostMemoryModel<T>::CreateArray(uint32_t width); \
	template T* HostMemoryModel<T>::CreateArray(uint32_t width, uint32_t height); \
	template T* HostMemoryModel<T>::CreateArray(uint32_t width, uint32_t height, uint32_t depth); \
	template T* HostMemoryModel<T>::Map(int32_t offset, size_t size); \
	template T* HostMemoryModel<T>::Map(); \
	template void HostMemoryModel<T>::UnMap();

#define INSTANTIATE_DEVICE_MEMORY_MODEL(T) \
	template class DeviceMemoryModel<T>; \
	template T* DeviceMemoryModel<T>::CreateArray(size_t width); \
	template T* DeviceMemoryModel<T>::Map(int32_t offset, size_t size); \
	template T* DeviceMemoryModel<T>::Map();
	//template void DeviceMemoryModel<T>::UnMap();

INSTANTIATE_MEMORY_MODEL(uint8_t);
INSTANTIATE_HOST_MEMORY_MODEL(uint8_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(uint8_t);

INSTANTIATE_MEMORY_MODEL(uint16_t);
INSTANTIATE_HOST_MEMORY_MODEL(uint16_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(uint16_t);

INSTANTIATE_MEMORY_MODEL(uint32_t);
INSTANTIATE_HOST_MEMORY_MODEL(uint32_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(uint32_t);

INSTANTIATE_MEMORY_MODEL(uint64_t);
INSTANTIATE_HOST_MEMORY_MODEL(uint64_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(uint64_t);

INSTANTIATE_MEMORY_MODEL(int8_t);
INSTANTIATE_HOST_MEMORY_MODEL(int8_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(int8_t);

INSTANTIATE_MEMORY_MODEL(int16_t);
INSTANTIATE_HOST_MEMORY_MODEL(int16_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(int16_t);

INSTANTIATE_MEMORY_MODEL(int32_t);
INSTANTIATE_HOST_MEMORY_MODEL(int32_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(int32_t);

INSTANTIATE_MEMORY_MODEL(int64_t);
INSTANTIATE_HOST_MEMORY_MODEL(int64_t);
INSTANTIATE_DEVICE_MEMORY_MODEL(int64_t);

INSTANTIATE_MEMORY_MODEL(float);
INSTANTIATE_HOST_MEMORY_MODEL(float);
INSTANTIATE_DEVICE_MEMORY_MODEL(float);

INSTANTIATE_MEMORY_MODEL(double);
INSTANTIATE_HOST_MEMORY_MODEL(double);
INSTANTIATE_DEVICE_MEMORY_MODEL(double);

INSTANTIATE_MEMORY_MODEL(bool);
INSTANTIATE_HOST_MEMORY_MODEL(bool);
INSTANTIATE_DEVICE_MEMORY_MODEL(bool);
/* ----------------------- Host Memory Model ------------------*/
template<class T> MemoryModel<T>* HostMemoryModel<T>::Clone(){
  int size = this->dims_[0]*this->dims_[1]*this->dims_[2];
  HostMemoryModel<T> *re= new HostMemoryModel<T>(this->num_dims_, this->alignment_);
  re->CreateArray(size);
  memcpy(re->mapped_memory_,this->mapped_memory_,size*sizeof(T));
  return re;
}

template<class T> T* HostMemoryModel<T>::CreateArray(uint32_t width)
{
	//this->memory_;
	this->dims_[0] = width;
	this->mapped_memory_ =  new T[width];
	this->mapped_ = true;
	return this->mapped_memory_;
}

template<class T> T* HostMemoryModel<T>::CreateArray(uint32_t width, uint32_t height){
	this->dims_[0] = width;
	this->dims_[1] = height;
	this->mapped_memory_ =  new T[width*height];
	this->mapped_ = true;
	return this->mapped_memory_;
}

template<class T> T* HostMemoryModel<T>::CreateArray(uint32_t width, uint32_t height, uint32_t depth){
	this->dims_[0] = width;
	this->dims_[1] = height;
	this->dims_[2] = depth;
	this->mapped_memory_ =  new T[width*height*depth];
	this->mapped_ = true;
	return this->mapped_memory_;
}

template<class T> T* HostMemoryModel<T>::Map(int32_t offset, size_t size){
	LOG4CXX_WARN( Sp::core_logger, "you are mapping a host memory model.");
	return NULL;
}
template<class T> T* HostMemoryModel<T>::Map(){
	LOG4CXX_WARN( Sp::core_logger, "you are mapping a device memory model.");
	return NULL;
}
template<class T> void HostMemoryModel<T>::UnMap(){
	LOG4CXX_ERROR( Sp::core_logger, "can not perform an unmap operation on host memory model.");
}
/* ----------------------- Memory Model -----------------------*/
template<class T> MemoryModel<T>* MemoryModel<T>::Clone(){
	//TODO: this function should do copy in host side instead of unmap and copy in device side.
	int err=0;
	MemoryModel<T>* re = new MemoryModel<T>(this->num_dims_,this->mapped_, this->pinned_, this->alignment_, this->access_mode_);
	re->SetImageFormat(this->image_format_);
	if(this->mapped_)
		this->UnMap();
	if(re->IsMapped())
		re->UnMap();

	cl::size_t<3> region, origin;
	switch(this->num_dims_){
	case 1:
		re->CreateArray(this->dims_[0]);
		this->queue_.enqueueCopyBuffer(this->buffer_,re->buffer_,0,0,(int)this->dims_[0]);
		break;
	case 2:
		re->CreateArray(this->dims_[0],this->dims_[1]);

		origin[0] = origin[1] = origin[2] = 0;
		region[0] = this->dims_[0]; region[1] = this->dims_[1]; region[2] = 1;
		err = this->queue_.enqueueCopyImage(this->image2d_,re->image2d_,origin,origin,region);
		if(err!=CL_SUCCESS)
			LOG4CXX_ERROR(Sp::core_logger,"unable to copy image, error code is: " << err);
		break;
	case 3:
		//TODO: 3D memory model cloning is unimplemented.
		break;
	default:
		break;
	}

	this->Map(0,0,this->dims_[0],this->dims_[1]);
	re->Map(0,0,this->dims_[0],this->dims_[1]);
	return re;
}

template<class T> T* MemoryModel<T>::CreateArray(uint32_t width) {
	if(!this->context_())
		this->context_ = DeviceManager::Instance().GetDefaultContext();
	this->dims_[0] = width;
	//TODO: currently, only CL_MEM_ALLOC_HOST_PTR constant is used.
	switch(this->num_dims_){
	case 1:
		this->buffer_ = cl::Buffer(this->context_, CL_MEM_ALLOC_HOST_PTR , width);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		return NULL;
	}
	this->memory_ = this->buffer_;
	return this->Map(0,width);
}

template<class T> T* MemoryModel<T>::CreateArray(uint32_t width, uint32_t height){
	if(!this->context_())
		this->context_ = DeviceManager::Instance().GetDefaultContext();
	int err=0;
	switch(this->num_dims_){
	case 2:

		this->image2d_ = cl::Image2D(this->context_,CL_MEM_ALLOC_HOST_PTR , this->image_format_, width, height, 0, NULL, &err);
		if(err!=CL_SUCCESS){
			LOG4CXX_ERROR(Sp::core_logger, "memory_model: error in creating image: " << err);
			return NULL;
		}
		break;
	default:
		return NULL;
	}
	this->dims_[0] = width;
	this->dims_[1] = height;
	this->memory_ = this->image2d_;
	return this->Map(0,0,width, height );
}

template<class T> T* MemoryModel<T>::Map(int32_t x, int32_t y, uint32_t width, uint32_t height){

	if(this->num_dims_!=2)
		return NULL;

	if(this->queue_()==NULL)
		this->queue_ = cl::CommandQueue(this->context_,DeviceManager::Instance().GetDefaultDevice());

	cl::size_t<3> region, origin;
	int err=0;
	origin[0] = origin[1] = origin[2] = 0;
	region[0] = width; region[1] = height; region[2] = 1;
	::size_t row_pitch =0, slice_pitch=0;

	this->mapped_memory_ = (T*) this->queue_.enqueueMapImage(this->image2d_,CL_TRUE,CL_MAP_READ | CL_MAP_WRITE, origin,region,&row_pitch,&slice_pitch, NULL, NULL, &err);
	if(err!=CL_SUCCESS)
		LOG4CXX_ERROR(Sp::core_logger, "memory_model: error in mapping image: " << err);
	this->mapped_ = true;
	return this->mapped_memory_;
}

template<class T> T* MemoryModel<T>::Map(int32_t offset, size_t size){
	if(this->queue_()==NULL)
		this->queue_ = cl::CommandQueue(this->context_,DeviceManager::Instance().GetDefaultDevice());

	switch(this->num_dims_){
	case 1:
		this->mapped_memory_ = (T*) this->queue_.enqueueMapBuffer(this->buffer_, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, offset, size);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		return NULL;
	}

	this->mapped_ = true;
	return this->mapped_memory_;
}

template<class T> T* MemoryModel<T>::Map(){
	return NULL;
}

template<class T> void MemoryModel<T>::UnMap(){
	if(!this->mapped_)
		return;
	this->queue_.enqueueUnmapMemObject(this->memory_, this->mapped_memory_);
	this->mapped_ = false;
}

/* ----------------------- Device Memory Model --------------------------*/
template<class T> T* DeviceMemoryModel<T>::CreateArray(uint32_t width){
	this->context_ = DeviceManager::Instance().GetDefaultContext();

	switch(this->num_dims_){
	case 1:
		this->buffer_ = cl::Buffer(this->context_, CL_MEM_READ_WRITE, width);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		return NULL;
	}
	this->memory_ = this->buffer_;
	return this->Map(0,width);
}

template<class T> T* DeviceMemoryModel<T>::Map(int32_t offset, size_t size){
	LOG4CXX_WARN( Sp::core_logger, "you are mapping on a device memory model. it's recommended to use mapping on MemoryModel only");
	Super::Map(offset, size);
}

template<class T> T* DeviceMemoryModel<T>::Map(){

}

//template<class T> void DeviceMemoryModel<T>::UnMap(){

//}

}
