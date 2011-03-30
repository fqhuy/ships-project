#ifndef __SHIPS_CORE_MATRIX_HPP__
#define __SHIPS_CORE_MATRIX_HPP__

namespace Sp {

/**
 * This class represents a mathematical matrix with some basic operations supported like Add, Mul, Det, Transpose..
 * @author Phan Quoc Huy (diepvien00thayh@gmail.com)
 * @param T1 matrix element type
 * @param T2 storage type
 */
template<class T1, class T2> class Matrix {
public:
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef const T1& ConstReference;
	typedef T1* Pointer;
	typedef Matrix<T1, T2> SelfType;
	Matrix();
	Matrix(Array<T2>* array, SampleModel<T1,T2>* sample_model);
	/**
	 * Matrix convenient constructor
	 * @param width ..
	 * @param height ..
	 */
	Matrix(const uint32_t& width, const uint32_t& height);
	Matrix(const uint32_t& width, const uint32_t& height, const uint32_t& mem_num_dims)	;
	/**
	 * Matrix full constructor
	 * @param width number of columns
	 * @param height number of rows
	 * @param memory_model the memory model backed behind the matrix
	 * @param sample_model the sample model that express the semantic of data stored inside Array
	 * @see Array
	 */
	Matrix(const uint32_t& width, const uint32_t& height, MemoryModel<
			ValueType2>* memory_model,
			SampleModel<ValueType1, ValueType2>* sample_model);
	virtual ~Matrix() {
		if(array_)
			delete array_;
		if(sample_model_)
			delete sample_model_;
	}
	virtual ValueType1 Get(const int& x, const int& y);
	virtual void Set(ValueType1 value, const int& x, const int& y);
	/*
	 * @brief this function quickly fill the matrix with zeros.
	 */
	virtual void Zeros();
	/*
	 * @brief this function init this matrix with the initial value from value parameter.
	 */
	virtual void Init(ValueType1 value);
	/*
	 * @brief clone an exactly the same matrix with different memory location.
	 */
	virtual Matrix<ValueType1,ValueType2>* Clone();

	SHIPS_INLINE
	virtual bool IsMapped(){
		return array_->IsMapped();
	}

	virtual uint32_t GetWidth();

	virtual uint32_t GetHeight();

	virtual Array<ValueType2>& GetArray();

	virtual Vector<ValueType1, ValueType2>* GetRow(const uint32_t& row);
	virtual Vector<ValueType1, ValueType2>* GetColumn(const uint32_t& column);

	virtual SelfType& Transpose();
	virtual SelfType& Assign(const SelfType& matrix);
	virtual SelfType& Add(const SelfType& matrix);
	virtual SelfType& Add(ConstReference value);
	virtual SelfType& Multiply(const SelfType& matrix);
	virtual SelfType& Multiply(ConstReference value);
	virtual ValueType1 Det();

	virtual std::string ToString();
protected:
	uint32_t width_;
	uint32_t height_;
	Array<ValueType2>* array_;
	SampleModel<ValueType1, ValueType2>* sample_model_;
private:
};

template<class T1, class T2> class HostMatrix: public Matrix<T1, T2> {
public:
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef const T1& ConstReference;
	typedef T1* Pointer;
	typedef HostMatrix<T1, T2> SelfType;
	typedef Matrix<T1, T2> Super;
	SHIPS_INLINE
	HostMatrix(HostArray<T2>* array, SampleModel<T1,T2>* sample_model) : Super(array, sample_model){

	}

	HostMatrix(const uint32_t& width, const uint32_t& height);

	SHIPS_INLINE
	HostMatrix(const uint32_t& width, const uint32_t& height, HostMemoryModel<
			ValueType2>* memory_model,
			SampleModel<ValueType1, ValueType2>* sample_model) :
		Super(width, height, memory_model, sample_model) {
	}

	virtual ~HostMatrix(){

	}

	Super* Clone();
protected:

private:

};

template<class T1, class T2> class DeviceMatrix: public Matrix<T1, T2> {
public:
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef const T1& ConstReference;
	typedef T1* Pointer;
	typedef DeviceMatrix<T1, T2> SelfType;
	DeviceMatrix();
	DeviceMatrix(HostArray<T2>* array, SampleModel<T1,T2>* sample_model) : Super(array, sample_model){
	}
	DeviceMatrix(const uint32_t& width, const uint32_t& height);
	DeviceMatrix(const uint32_t& width, const uint32_t& height, MemoryModel<
			ValueType2>* memory_model,
			SampleModel<ValueType1, ValueType2>* sample_model) :
		Super(width, height, memory_model, sample_model) {
	}
	virtual ~DeviceMatrix() {
	}
private:
	typedef Matrix<T1, T2> Super;
};

}

#endif
