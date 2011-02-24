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
	/**
	 * Matrix convenient constructor
	 * @param width ..
	 * @param height ..
	 */
	Matrix(const uint32_t& width, const uint32_t& height);
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
		delete array_;
		delete sample_model_;
	}
	virtual ValueType1 Get(int x, int y);
	virtual void Set(ValueType1 value, int x, int y);

	virtual uint32_t GetWidth();

	virtual uint32_t GetHeight();

	virtual Array<ValueType2>& GetArray();

	virtual SelfType& Transpose();
	virtual SelfType& Assign(const SelfType& matrix);
	virtual SelfType& Add(const SelfType& matrix);
	virtual SelfType& Add(ConstReference value);
	virtual SelfType& Multiply(const SelfType& matrix);
	virtual SelfType& Multiply(ConstReference value);
	virtual ValueType1 Det();
protected:
	uint32_t width_;
	uint32_t height_;
	Array<ValueType2>* array_;
	SampleModel<ValueType1, ValueType2>* sample_model_;
private:
};

template<class T1, class T2> class HostMatrix: public Matrix<T1, T2> {
public:

protected:

private:

};

template<class T1, class T2> class DeviceMatrix: public Matrix<T1, T2> {
};

}

#endif
