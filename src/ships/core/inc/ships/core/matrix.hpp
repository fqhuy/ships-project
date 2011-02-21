#ifndef __SHIPS_CORE_MATRIX_HPP__
#define __SHIPS_CORE_MATRIX_HPP__

namespace Sp {

//Matrix adapter to adapt various kinds of matrix like ublas, viennal ...
//equipvalent to java.awt.image.Raster
/**
 * T1: matrix element type
 * T2: storage type
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
	Matrix(const uint32_t& width, const uint32_t& height);
	Matrix(const uint32_t& width, const uint32_t& height,
			MemoryModel<ValueType2>* memory_model, SampleModel<ValueType1, ValueType2>* sample_model);
	virtual ~Matrix() {
		delete array_;
		delete sample_model_;
	}
	virtual ValueType1 Get(int x, int y);
	virtual void Set(ValueType1 value, int x, int y);

	virtual uint32_t GetWidth();

	virtual uint32_t GetHeight();

	virtual SelfType& Transpose();
	virtual SelfType& Assign(const SelfType& matrix);
	virtual SelfType& Add(const SelfType& matrix);
	virtual SelfType& Add(ConstReference value);
	virtual SelfType& Multiply(const SelfType& matrix);
	virtual SelfType& Multiply(ConstReference value);
	virtual ValueType1 Det();
protected:
	Array<ValueType2>* array_;
	SampleModel<ValueType1, ValueType2>* sample_model_;
private:
};

template<class T1, class T2> class HostMatrix: public Matrix<T1, T2 > {
public:

protected:

private:

};

template<class T1, class T2> class DeviceMatrix: public Matrix<T1, T2> {
};

}

#endif
