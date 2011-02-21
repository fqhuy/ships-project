/*
 * array.hpp
 *
 *  Created on: Feb 12, 2011
 *      Author: fqhuy
 */

#ifndef ARRAY_HPP_
#define ARRAY_HPP_

namespace Sp {

template<class T> class Array {
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T* Pointer;
	/*
	SHIPS_INLINE
	Array(int width, int height, ValueType init_value=0){
		MemoryModel<ValueType>* mm = new MemoryModel<ValueType>(2);
		uint32_t dims[]= {width, height};
		Init(2, dims, mm, init_value);
	}
	*/
	/*
	 * Array constructor
	 * @param num_dims the number of dimensions of the array.
	 * @param dims array of sizes of this Array e.g. width, height, depth ...
	 * @param memory_model the memory model of this Array.
	 * @param init_value the initial value of the array, default is 0.
	 */
	SHIPS_INLINE
	Array(int width, int height, MemoryModel<ValueType>* memory_model, ValueType init_value=0){
		uint32_t dims[]= {width, height};
		Init(2, dims, memory_model, init_value);
	}
	/*
	 * Array constructor
	 * @param num_dims the number of dimensions of the array.
	 * @param dims array of sizes of this Array e.g. width, height, depth ...
	 * @param memory_model the memory model of this Array.
	 * @param init_value the initial value of the array, default is 0.
	 */
	SHIPS_INLINE
	Array(int num_dims, uint32_t* dims,
			MemoryModel<ValueType>* memory_model, ValueType init_value = 0) {
		Init(num_dims, dims, memory_model, init_value);
	}

	virtual ~Array() {
		data_ = NULL;
		delete steps_;
		delete dims_;
	}

	virtual void Resize(uint32_t* dims);
	virtual Array<ValueType>* Clone();
	virtual Array<ValueType>& Assign(const Array<ValueType>& array);

	SHIPS_INLINE
	uint32_t GetWidth() {
		return width_;
	}

	SHIPS_INLINE
	uint32_t GetHeight() {
		return height_;
	}

	SHIPS_INLINE
	uint32_t GetDepth() {
		return depth_;
	}

	SHIPS_INLINE
	size_t GetSize() {
		size_t re=1;
		for (int i=0; i< num_dims_; i++)
			re*=dims_[i];
		return re;
	}

	SHIPS_INLINE
	virtual ValueType Get(const int*& indices) {
		int index = 0;
		for (int i = 0; i < this->num_dims_; i++) {
			index += steps_[i] * indices[this->num_dims_ - i];
		}
		return data_[index];
	}

	SHIPS_INLINE
	virtual ValueType Get(int index) {
		return data_[index];
	}

	SHIPS_INLINE
	virtual ValueType Get(int x, int y) {
		return data_[steps_[0] * y + x];
	}

	SHIPS_INLINE
	virtual ValueType Get(int x, int y, int z) {
		return data_[steps_[0] * z + steps_[1] * y + x];
	}

	SHIPS_INLINE
	virtual void Set(ValueType value, const int*& indices) {
		int index = 0;
		for (int i = 0; i < this->num_dims_; i++) {
			index += steps_[i] * indices[this->num_dims_ - i];
		}
		data_[index] = value;
	}

	SHIPS_INLINE
	virtual void Set(ValueType value, int index) {
		data_[index] = value;
	}

	SHIPS_INLINE
	virtual void Set(ValueType value, int x, int y) {
		data_[steps_[0] * y + x] = value;
	}

	SHIPS_INLINE
	virtual void Set(ValueType value, int x, int y, int z) {
		data_[steps_[0] * z + steps_[1] * y + x] = value;
	}
protected:
	cl_uint* steps_;
	Pointer data_;
	Pointer* data2d_;
	Pointer** data3d_;
	cl_uint num_dims_;
	cl_uint width_, height_, depth_;
	cl_uint* dims_;
	MemoryModel<ValueType>* memory_model_;

	void Init(uint32_t num_dims, uint32_t* dims,MemoryModel<ValueType>* memory_model, ValueType init_value = 0);

	SHIPS_INLINE
	bool CheckCompatability(){
		if(num_dims_ == memory_model_->GetNumDims())
			return true;

		return false;
	}
};

template<class T>
class HostArray: public Array<T> {
public:
	typedef Array<T> Super;
	typedef HostArray<T> SelfType;
	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T* Pointer;
	typedef const T* ConstPointer;

	SHIPS_INLINE
	HostArray(int num_dims, uint32_t* dims,
			HostMemoryModel<ValueType>* memory_model, ValueType init_value = 0) :
		Super (num_dims, dims, memory_model, init_value) {

	}

	SHIPS_INLINE
	HostArray(int num_dims, uint32_t* dims,
			HostMemoryModel<ValueType>* memory_model, ConstPointer data) //:Super (num_dims, dims, memory_model)
	{
		this->data_ = data;
	}

	SHIPS_INLINE
	Super* Clone(){
		Pointer data = new ValueType[this->GetSize()];
		std::copy(this->data_,this->data_ + this->GetSize(), data);
		HostMemoryModel<ValueType>* hmm = new HostMemoryModel<ValueType>();
		SelfType* re = new SelfType(this->num_dims_, this->dims_, this->memory_model_, hmm, data );
		return re;
	}
protected:

};

template<class T>
class DeviceArray: public Array<T> {
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T* Pointer;

	SHIPS_INLINE
	DeviceArray(int num_dims, uint32_t* dims,
			DeviceMemoryModel<ValueType>* memory_model, ValueType init_value = 0) :
		Array<T> (num_dims, dims, memory_model, init_value) {

	}
};
}
#endif /* ARRAY_HPP_ */
