/*
 * sample_model.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */
#include <ships/core/core.hpp>

namespace Sp {
#define INSTANTIATE_PIXELINTERLEAVEDSAMPLEMODEL(T) \
		template Array<T>* PixelInterleavedSampleModel<T>::CreateArray(MemoryModel<T>* memory_model);
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

template<class T> Array<T>* PixelInterleavedSampleModel<T>::CreateArray(
		MemoryModel<T>* memory_model) {

	if (memory_model->GetNumDims() == 1) {
		uint32_t size = this->CalculateBufferSize(memory_model->GetAlignment());

		Array<T>* re =  new Array<T> (1, &size, memory_model, 0);
		//fill the padded portion by 0
		if(size > this->dims_[0]){
			for(int i=this->dims_[0];i<size;i++)
				re->Set(0,i);
		}
		return re;
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

		return new Array<T> (this->num_dims_, this->dims_, memory_model, 0);
	}

	return NULL;
}
}