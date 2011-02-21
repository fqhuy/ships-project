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

template<class T1, class T2> class SampleModel {
public:
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef T1* Pointer;
	typedef const T1& ConstReference;

	virtual ~SampleModel() {
		delete dims_;
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
	uint32_t* GetDims() {
		return dims_;
	}
	/**
	 *
	 */
	virtual Array<ValueType2>* CreateArray(
			MemoryModel<ValueType2>* memory_model) = 0;
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @param y y position.
	 * @param z z position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array, int x, int y, int z,
			int channel = 0) =0;
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @param y y position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array, int x, int y,
			int channel = 0) =0;
	/**
	 * @param array the array that this model will look for a specific sample.
	 * @param channel sample channel.
	 * @param x x position.
	 * @return the sample value from input array.
	 */
	virtual ValueType1 GetSample(Array<ValueType2>* array, int x, int channel =
			0)=0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, int x,
			int channel = 0) =0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, int x,
			int y, int channel = 0) =0;
	/**
	 *
	 */
	virtual void SetSample(Array<ValueType2>* array, ValueType1 value, int x,
			int y, int z, int channel = 0)=0;
	/**
	 * @param array the array that this model will look for samples
	 * @param x
	 * @param y
	 * @param z
	 * @return a pointer to an array of sample. size of this array is equal to the number of channels.
	 */
	virtual Pointer GetSamples(Array<ValueType2>* array, int x, int y, int z) {
		Pointer re = new ValueType1[3];
		re[0] = GetSample(array, x * this->num_channels_, y, z, 0);
		re[1] = GetSample(array, x * this->num_channels_ + 1, y, z, 1);
		re[2] = GetSample(array, x * this->num_channels_ + 2, y, z, 2);
		return re;
	}

	virtual void SetSamples(Array<ValueType2>* array, Pointer values, int x,
			int y, int z) {
		SetSample(array, values[0], x, y, z, 0);
		SetSample(array, values[1], x, y, z, 1);
		SetSample(array, values[2], x, y, z, 2);
	}
protected:
	uint32_t num_channels_;
	uint32_t num_dims_;
	uint32_t* dims_;

	SHIPS_INLINE SampleModel(uint32_t num_channels, uint32_t num_dims,
			uint32_t* dims) :
		num_channels_(num_channels), num_dims_(num_dims), dims_(dims) {
	}
	/**
	 * @param width the width of the area of the array which this sample model describe
	 * @param height the height of the area
	 * @param depth
	 * @param num_channels the number of channels
	 */
	//SHIPS_INLINE SampleModel(uint32_t width, uint32_t height, uint32_t depth,
	//		uint32_t num_channels) {
	//	num_channels_ = num_channels;
	//dims_ = new uint32_t[]
	//}
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
	/*
	 * CL_R, CL_Rx or CL_A
	 * CL_INTENSITY. This format can only be used if channel data type = CL_UNORM_INT8,
	 * CL_UNORM_INT16, CL_SNORM_INT8, CL_SNORM_INT16, CL_HALF_FLOAT or
	 CL_FLOAT.
	 CL_LUMINANCE. This format can only be used if channel data type =
	 CL_UNORM_INT8, CL_UNORM_INT16, CL_SNORM_INT8, CL_SNORM_INT16,
	 CL_HALF_FLOAT or CL_FLOAT.
	 CL_RG, CL_RGx or CL_RA
	 CL_RGB or CL_RGBx. This format can only be used if channel data type =
	 CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
	 CL_RGBA
	 CL_ARGB, CL_BGRA. This format can only be used if channel data type =
	 CL_UNORM_INT8, CL_SNORM_INT8, CL_SIGNED_INT8 or CL_UNSIGNED_INT8.
	 *
	 */
	Array<ValueType>* CreateArray(MemoryModel<ValueType>* memory_model);

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, int x, int channel = 0) {
		int index = x * (this->num_channels_) + channel;
		return array->Get(index);
	}

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, int x, int y, int channel = 0) {
		return array->Get(x * (this->num_channels_) + channel, y);
	}

	SHIPS_INLINE
	ValueType GetSample(Array<ValueType>* array, int x, int y, int z,
			int channel = 0) {
		return array->Get(x * (this->num_channels_) + channel, y, z);
	}

	void SetSample(Array<ValueType>* array, ValueType value, int x,
			int channel = 0) {
	}
	/**
	 *
	 */
	void SetSample(Array<ValueType>* array, ValueType value, int x, int y,
			int channel = 0) {
	}
	/**
	 *
	 */
	void SetSample(Array<ValueType>* array, ValueType value, int x, int y,
			int z, int channel = 0) {
	}
private:
	typedef ComponentSampleModel<T> Super;
};

}

#endif /* SAMPLE_MODEL_HPP_ */
