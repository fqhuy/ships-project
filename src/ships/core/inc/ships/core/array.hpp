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

	SHIPS_INLINE
	Array(uint32_t size, ValueType init_value = 0) {
		MemoryModel<ValueType>* mm = new MemoryModel<ValueType>(2);
		uint32_t dims[]= {size,1};
		Init(1, dims, mm, init_value);
	}
	SHIPS_INLINE
	Array(uint32_t size, MemoryModel<ValueType>* memory_model, ValueType init_value = 0) {
		uint32_t dims[]= {size,1};
		Init(1, dims, memory_model, init_value);
	}
	SHIPS_INLINE
	Array(const uint32_t& width, const uint32_t& height, ValueType init_value=0) {
		MemoryModel<T>* mm = new MemoryModel<T>(2,true,true,1,READ_WRITE);
		uint32_t dims[] = { width, height };
		Init(2, dims, mm, true, init_value);
	}
	/*
	 * Array constructor
	 * @param num_dims the number of dimensions of the array.
	 * @param dims array of sizes of this Array e.g. width, height, depth ...
	 * @param memory_model the memory model of this Array.
	 * @param init_value the initial value of the array, default is 0.
	 */
	SHIPS_INLINE
	Array(const uint32_t& width, const uint32_t& height,
			MemoryModel<ValueType>* memory_model, ValueType init_value = 0) {
		uint32_t dims[] = { width, height };
		Init(2, dims, memory_model, true, init_value);
	}
	/*
	 * Array constructor
	 * @param num_dims the number of dimensions of the array.
	 * @param dims array of sizes of this Array e.g. width, height, depth ...
	 * @param memory_model the memory model of this Array.
	 * @param init_value the initial value of the array, default is 0.
	 */
	SHIPS_INLINE
	Array(const uint32_t&  num_dims, uint32_t* dims,
			MemoryModel<ValueType>* memory_model, ValueType init_value = 0) {
		Init(num_dims, dims, memory_model, true, init_value);
	}

	SHIPS_INLINE
	Array(const uint32_t&  num_dims, uint32_t* dims,
			MemoryModel<ValueType>* memory_model, Pointer data) {
		this->data_ = data;
		Init(num_dims, dims, memory_model, false);
	}

	virtual ~Array() {
		delete memory_model_;
		data_ = NULL;
	}

	virtual void Resize(uint32_t* dims);
	virtual Array<ValueType>* Clone();
	virtual Array<ValueType>& Assign(const Array<ValueType>& array);

	SHIPS_INLINE
	virtual bool IsMapped(){
		return memory_model_->IsMapped();
	}

	virtual void Map();
	virtual void UnMap();

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
		size_t re = 1;
		for (int i = 0; i < num_dims_; i++)
			re *= dims_[i];
		return re;
	}

	SHIPS_INLINE
	MemoryModel<ValueType>& GetMemoryModel() {
		return *memory_model_;
	}

	SHIPS_INLINE
	Pointer GetData() {
		return data_;
	}
	SHIPS_INLINE
	Pointer* GetData2D() {
		return data2d_;
	}
	SHIPS_INLINE
	Pointer** GetData3D() {
		return data3d_;
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
		try {
			if (memory_model_->IsMapped())
				return data_[index];
			else
				throw "the memory isn't mapped.";
		} catch (char* msg) {
			LOG4CXX_ERROR(Sp::core_logger, msg);
		}

	}

	SHIPS_INLINE
	virtual ValueType Get(int x, int y) {
		try {
			if (memory_model_->IsMapped())
				return data2d_[x][y];
			else
				throw "the memory isn't mapped.";
		} catch (char* msg) {
			LOG4CXX_ERROR(Sp::core_logger, msg);
		}
	}
	//TODO: another try catch here.
	SHIPS_INLINE
	virtual ValueType Get(int x, int y, int z) {
		return data3d_[x][y][z];
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
		data2d_[x][y] = value;
	}

	SHIPS_INLINE
	virtual void Set(ValueType value, int x, int y, int z) {
		data3d_[x][y][z] = value;
	}

	virtual std::string ToString();
protected:
	//Max number of dimensions supported is 16 ^_^
	cl_uint steps_[SP_MAX_NUM_DIMENSIONS];
	Pointer data_;
	Pointer* data2d_;
	Pointer** data3d_;
	cl_uint num_dims_;
	cl_uint width_, height_, depth_;
	cl_uint dims_[SP_MAX_NUM_DIMENSIONS];
	MemoryModel<ValueType>* memory_model_;

	void Init(uint32_t num_dims, uint32_t* dims,
			MemoryModel<ValueType>* memory_model, bool create_data = true, ValueType init_value = 0);

	SHIPS_INLINE
	bool CheckCompatability() {
		if (num_dims_ == memory_model_->GetNumDims())
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
	HostArray(const uint32_t& width, const uint32_t& height, ValueType init_value=0) {
		MemoryModel<T>* mm = new HostMemoryModel<T>(2,1);
		uint32_t dims[] = { width, height };
		this->Init(2, dims, mm, true, init_value);
	}

	SHIPS_INLINE
	HostArray(const uint32_t& width,const uint32_t& height,
			MemoryModel<ValueType>* memory_model, ValueType init_value = 0): Super(width, height, memory_model, init_value){

	}

	SHIPS_INLINE
	HostArray(const uint32_t& num_dims, uint32_t* dims, MemoryModel<
			ValueType>* memory_model, ValueType init_value = 0) :
		Super(num_dims, dims, memory_model, init_value) {

	}

	SHIPS_INLINE
	HostArray(const uint32_t&  num_dims, uint32_t* dims, MemoryModel<
			ValueType>* memory_model, Pointer data) :Super (num_dims, dims, memory_model, data)
	{

	}
	/*
	SHIPS_INLINE
	Super* Clone() {
		Pointer data = new ValueType[this->GetSize()];
		std::copy(this->data_, this->data_ + this->GetSize(), data);
		HostMemoryModel<ValueType>* hmm = new HostMemoryModel<ValueType> ();
		SelfType* re = new SelfType(this->num_dims_, this->dims_,
				this->memory_model_, hmm, data);
		return re;
	}
	*/
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
	DeviceArray(const uint32_t& width, const uint32_t& height, ValueType init_value=0) {
		MemoryModel<T>* mm = new DeviceMemoryModel<T>(2);
		uint32_t dims[] = { width, height };
		this->Init(2, dims, mm, true, init_value);
	}

	SHIPS_INLINE
	DeviceArray(const uint32_t& width,const uint32_t& height,
			MemoryModel<ValueType>* memory_model, ValueType init_value = 0):
			Super(width, height, memory_model, init_value){

	}
	SHIPS_INLINE DeviceArray(int num_dims, uint32_t* dims, MemoryModel<
			ValueType>* memory_model, ValueType init_value = 0) :
		Super (num_dims, dims, memory_model, init_value) {
	}
private:
	typedef Array<T> Super;
};
}
#endif /* ARRAY_HPP_ */
