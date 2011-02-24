/*
 * array.cpp
 *
 *  Created on: Feb 12, 2011
 *      Author: fqhuy
 */

#include <ships/core/core.hpp>
#include <ships/core/memory_model.hpp>
namespace Sp{

#define INSTANTIATE_ARRAY(T) \
	template class Array<T>; \
	template void Array<T>::Resize(uint32_t* dims); \
	template Array<T>* Array<T>::Clone(); \
	template Array<T>& Array<T>::Assign(const Array<T>& array);

INSTANTIATE_ARRAY(int8_t);
INSTANTIATE_ARRAY(uint8_t);
INSTANTIATE_ARRAY(int16_t);
INSTANTIATE_ARRAY(uint16_t);
INSTANTIATE_ARRAY(int32_t);
INSTANTIATE_ARRAY(uint32_t);
INSTANTIATE_ARRAY(int64_t);
INSTANTIATE_ARRAY(uint64_t);
INSTANTIATE_ARRAY(float);
INSTANTIATE_ARRAY(double);

template<class T> void Array<T>::Resize(uint32_t* dims){

}

template<class T> Array<T>* Array<T>::Clone(){

}

template<class T> Array<T>& Array<T>::Assign(const Array<T>& array){

}

template<class T> void Array<T>::Init(uint32_t num_dims, uint32_t* dims,
			MemoryModel<T>* memory_model, T init_value){
	num_dims_ = num_dims;
	memory_model_ = memory_model;
	dims_ = dims;

	switch(num_dims){
	case 1:
		data_ = memory_model_->CreateArray(dims[0]);
		if(data_==NULL)
			LOG4CXX_ERROR(Sp::core_logger, "array.cpp: error in creating data_");
		break;
	case 2:
		// TODO: fix this
		data_ = memory_model_->CreateArray(dims[0], dims[1]);
		break;
	case 3:
		// TODO: fix this
		data_ = memory_model_->CreateArray(dims[0]*dims[1]*dims[2]);
		break;
	default:
		break;
	}

	if (num_dims == 1) {
		width_ = dims[0];
		height_ = 0;
		depth_ = 0;
	} else if (num_dims == 2) {
		width_ = dims[0];
		height_ = dims[1];
		depth_ = 0;

		data2d_ = new T*[height_];
		for(int i=0;i<height_;i++){
			data2d_[i] = data_;
			data2d_[i] = data2d_[i] + width_ * i;
		}

	} else if (num_dims == 3) {
		width_ = dims[0];
		height_ = dims[1];
		depth_ = dims[2];

		data3d_ = new T**[depth_];
		for(int i=0;i<depth_;i++){
			data3d_[i] = new T*[height_];
			for(int j=0;j<height_;j++){
				data3d_[i][j] = data_;
				data3d_[i][j] = data3d_[i][j] + width_ * height_ * i + width_ * j;
			}
		}
	}

	steps_ = new uint32_t[num_dims];
	int total = 1;
	int i;
	for (i = 0; i < num_dims; i++) {
		total *= dims[i];
	}
	steps_[0] = total / dims[num_dims - 1];
	for (i = 1; i < num_dims; i++) {
		steps_[i] = steps_[i - 1] / dims[num_dims - i - 1];
	}
}


}
