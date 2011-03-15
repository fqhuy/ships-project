/*
 * sample_model.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */
#include <ships/core/core.hpp>
#include <tiff.h>

namespace Sp {
#define INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(T) \
		template Array<T>* PixelInterleavedSampleModel<T>::CreateArray(MemoryModel<T>* memory_model); \
		template SampleModel<T,T>* PixelInterleavedSampleModel<T>::Clone();
		
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(int8_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(uint8_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(int16_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(uint16_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(int32_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(uint32_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(int64_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(uint64_t)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(float)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(double)
INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(bool)

template<class T> SampleModel<T,T>* PixelInterleavedSampleModel<T>::Clone(){
  uint32_t dims[SP_MAX_NUM_DIMENSIONS];
  ArrayUtils::copyArray<uint32_t>(0,SP_MAX_NUM_DIMENSIONS,this->dims_,dims);
  SampleModel<T,T>* re  = new PixelInterleavedSampleModel<T>(this->num_channels_,this->num_dims_,dims);
}

template<class T> Array<T>* PixelInterleavedSampleModel<T>::CreateArray(
		MemoryModel<T>* memory_model) {

	uint32_t dims[16];
	ArrayUtils::copyArray(0,SP_MAX_NUM_DIMENSIONS,this->dims_,dims);

	if (memory_model->GetNumDims() == 1) {
		//uint32_t size = this->CalculateBufferSize(memory_model->GetAlignment());
		//Array<T>* re=NULL;
		dims[0] = this->dims_[0]*this->num_channels_;
		//re =  new Array<T> (this->num_dims_, dims, memory_model, (T)0);
		//return re;
	}

	if (memory_model->GetNumDims() == 2 || memory_model->GetNumDims() == 3) {
		cl::ImageFormat imf = cl::ImageFormat();
		imf.image_channel_data_type = Type2Const<T>::type;
		switch (this->num_channels_) {
		case 1:
			imf.image_channel_order = CL_A;
			break;
		case 2:
			imf.image_channel_order = CL_RG;
			break;
		case 3:
			imf.image_channel_order = CL_RGB;
			break;
		case 4:
			imf.image_channel_order = CL_RGBA;
			break;
		default:
			imf.image_channel_order = CL_A;
			break;
		}
		memory_model->SetImageFormat(imf);
	}

	try{
		MemoryModel<T>* m1 = dynamic_cast<HostMemoryModel<T>* >(memory_model);
		if(m1!=NULL)
			return new HostArray<T> (this->num_dims_, (uint32_t*) dims, memory_model, (T) 0);

		m1 = dynamic_cast<DeviceMemoryModel<T>* >(memory_model);
		if(m1!=NULL)
			return new DeviceArray<T> (this->num_dims_, (uint32_t*)dims, memory_model, (T) 0);


	} catch (std::exception& e){
		LOG4CXX_ERROR(core_logger,e.what());
	}

	return new Array<T> (this->num_dims_, (uint32_t*)dims, memory_model, (T) 0);
}
}
