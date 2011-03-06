/*
 * sample_model.hpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */

#ifndef SAMPLE_MODEL_HPP_
#define SAMPLE_MODEL_HPP_

namespace Sp {
//TODO: some more types is still missing.
template<class T> struct Type2Const{
	enum {type = 0, norm_type = 0};
};
template<> struct Type2Const<int8_t>{
	enum {type = CL_SIGNED_INT8, norm_type = CL_SNORM_INT8};
};
template<> struct Type2Const<uint8_t>{
	enum {type = CL_UNSIGNED_INT8, norm_type = CL_UNORM_INT8};
};
template<> struct Type2Const<int16_t>{
	enum {type = CL_SIGNED_INT16, norm_type = CL_SNORM_INT16};
};
template<> struct Type2Const<uint16_t>{
	enum {type = CL_UNSIGNED_INT16, norm_type = CL_UNORM_INT16};
};
template<> struct Type2Const<int32_t>{
	enum {type = CL_SIGNED_INT32, norm_type = CL_SNORM_INT8};
};
template<> struct Type2Const<uint32_t>{
	enum {type = CL_UNSIGNED_INT32, norm_type = CL_UNORM_INT8};
};
template<> struct Type2Const<float>{
	enum {type = CL_FLOAT};
};
template<> struct Type2Const<double>{
	enum {type = CL_FLOAT};
};
/**
 * this class express the structure of underlined raw data.
 * @author Phan Quoc Huy (diepvien00thayh@gmail.com)
 * @param T1 value type
 * @param T2 storage type
 */
template<class T1, class T2> class SampleModel {
public:
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef T1* Pointer;
	typedef const T1& ConstReference;

	virtual ~SampleModel() {
		//delete dims_;
	}

	SHIPS_INLINE
	uint32_t GetNumChannels() {
		return num_channels_;
	}

	SHIPS_INLINE
	uint32_t GetNumDims() {
		return num_dims_;
	}

	SHIPS_INLINE
	uint32_t *GetDims() {
		return dims_;
	}
	/*
	 * @brief clone the current sample model
	 */ 
	virtual SampleModel<ValueType1,ValueType2>* Clone() = 0;
	/**
	 *
	 */
	virtual Array<ValueType2>* CreateArray(
			MemoryModel<ValueType2>* memory_model) = 0;
	/**
	 * @return size of the area in number of elements
	 */
	SHIPS_INLINE
	uint32_t GetSize(){
		uint32_t re=1;
		for(int i=0;i<num_dims_;i++){
			re*=dims_[i];
		}
		return re;
	}
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @param y y position.
	 * @param z z position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array, const int& x, const int& y, const int& z,
			const int& channel) =0;
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @param y y position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array, const int& x, const int& y,
			const int& channel) =0;
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array,const int& x, const int& channel )=0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, const int& x,
			const int& channel) =0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, const int& ,
			const int& y, const int& channel) =0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, const int& ,
			const int& y, const int&  z, const int& channel)=0;
	/**
	 * @param array the array that this model will look for samples
	 * @param x
	 * @param y
	 * @param z
	 * @return a pointer to an array of sample. size of this array is equal to the number of channels.
	 */
	virtual Pointer GetSamples(Array<ValueType2>* array, const int& x, const int& y, const int& z) {
		Pointer re = new ValueType1[3];
		re[0] = GetSample(array, x * this->num_channels_, y, z, 0);
		re[1] = GetSample(array, x * this->num_channels_ + 1, y, z, 1);
		re[2] = GetSample(array, x * this->num_channels_ + 2, y, z, 2);
		return re;
	}

	virtual void SetSamples(Array<ValueType2>* array, Pointer values, const int& x,
			const int& y, const int& z) {
		SetSample(array, values[0], x, y, z, 0);
		SetSample(array, values[1], x, y, z, 1);
		SetSample(array, values[2], x, y, z, 2);
	}
protected:
	uint32_t num_channels_;
	uint32_t num_dims_;
	uint32_t dims_[SP_MAX_NUM_DIMENSIONS];

	SHIPS_INLINE
	SampleModel(uint32_t num_channels, uint32_t num_dims,
			uint32_t* dims) :
		num_channels_(num_channels), num_dims_(num_dims) {
		ArrayUtils::copyArray<uint32_t>(0,num_dims,dims,dims_);
		ArrayUtils::initArray(num_dims,(uint32_t)SP_MAX_NUM_DIMENSIONS,(uint32_t)1,dims_);
	}

	SHIPS_INLINE
	virtual uint32_t CalculateBufferSize(uint8_t alignment){
		uint32_t size = num_channels_;
		if(!dims_)
			return 0;
		for(int i=0;i<num_dims_;i++){
			size*=dims_[i];
		}
		if(size % alignment==0){
			return size;
		} else {
			size = (size/alignment + 1)*alignment;
			return size;
		}
	}
};

template<class T> class ComponentSampleModel: public SampleModel<T, T> {
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef const T& ConstReference;

	ComponentSampleModel(uint32_t num_channels, uint32_t num_dims,
			uint32_t* dims) :
		Super(num_channels, num_dims, dims) {
	}

	virtual ~ComponentSampleModel() {
	}
private:
	typedef SampleModel<T, T> Super;
};

template<class T> class PixelInterleavedSampleModel: public ComponentSampleModel<
		T> {
public:
	typedef T ValueType;
	typedef T* Pointer;
	PixelInterleavedSampleModel(uint32_t num_channels, uint32_t num_dims,
			uint32_t* dims) :
		Super(num_channels, num_dims, dims) {
	}

	virtual ~PixelInterleavedSampleModel() {
	}
	/**
	 * CL_R, CL_Rx or CL_A
	 * CL_INTENSITY. This format can only be used if channel data type = CL_UNORM_INT8,
	 * CL_UNORM_INT16, CL_SNORM_INT8, CL_SNORM_INT16, CL_HALF_FLOAT or
	 * CL_FLOAT.
	 * CL_LUMINANCE. This format can only be used if channel data type =
	 * CL_UNORM_INT8, CL_UNORM_INT16, CL_SNORM_INT8, CL_SNORM_INT16,
	 * CL_HALF_FLOAT or CL_FLOAT.
	 * CL_RG, CL_RGx or CL_RA
	 * CL_RGB or CL_RGBx. This format can only be used if channel data type =
	 * CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
	 * CL_RGBA
	 * CL_ARGB, CL_BGRA. This format can only be used if channel data type =
	 * CL_UNORM_INT8, CL_SNORM_INT8, CL_SIGNED_INT8 or CL_UNSIGNED_INT8.
	 * @param memory_model memory model to create the data buffer
	 * @return pointer to Array object
	 */
	SampleModel<ValueType,ValueType>* Clone();
	Array<ValueType>* CreateArray(MemoryModel<ValueType>* memory_model);

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, const int& x, const int& channel = 0) {
		int index = x * (this->num_channels_) + channel;
		return array->Get(index);
	}

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, const int& x, const int& y, const int& channel = 0) {
		return array->Get(x * (this->num_channels_) + channel, y);
	}

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, const int& x, const int& y, const int& z,
			const int& channel = 0) {
		return array->Get(x * (this->num_channels_) + channel, y, z);
	}
	/**
	 * @see SampleModel
	 */
	SHIPS_INLINE
	void SetSample(Array<ValueType>* array, ValueType value, const int& x,
			const int& channel = 0) {
		array->Set(value,x * (this->num_channels_) + channel);
	}
	/**
	 *
	 */
	SHIPS_INLINE
	void SetSample(Array<ValueType>* array, ValueType value, const int& x, const int& y,
			const int& channel = 0) {
		array->Set(value,x * (this->num_channels_) + channel,y);
	}
	/**
	 *
	 */
	SHIPS_INLINE
	void SetSample(Array<ValueType>* array, ValueType value, const int& x, const int& y,
			const int& z, const int& channel = 0) {
		array->Set(value,x * (this->num_channels_) + channel,y,z);
	}
private:
	typedef ComponentSampleModel<T> Super;
};

}

#endif /* SAMPLE_MODEL_HPP_ */
