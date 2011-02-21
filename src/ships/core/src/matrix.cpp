#include <ships/core/core.hpp>

namespace Sp {

#define INSTANTIATE_MATRIX(T1, T2) \
	template class Matrix<T1, T2>; \
	template Matrix<T1, T2>::Matrix(); \
	template Matrix<T1, T2>::Matrix(const uint32_t& width, const uint32_t& height); \
	template Matrix<T1, T2>::Matrix(const uint32_t& width,const uint32_t& height, MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model); \
	template uint32_t Matrix<T1, T2>::GetWidth(); \
	template uint32_t Matrix<T1, T2>::GetHeight(); \
	template T1 Matrix<T1, T2>::Get(int x, int y); \
	template void Matrix<T1, T2>::Set(T1 value, int x, int y); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Matrix<T1, T2>::Transpose(); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Assign(const Matrix<T1, T2>& matrix); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Add(const Matrix<T1, T2>& matrix); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Add(const T1& value); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Multiply(const Matrix<T1, T2>& matrix); \
	template Matrix<T1, T2>& Matrix<T1, T2>::Multiply(const T1& value); \
	template T1 Matrix<T1, T2>::Det();

INSTANTIATE_MATRIX(uint8_t, uint8_t);
INSTANTIATE_MATRIX(int8_t, int8_t);
INSTANTIATE_MATRIX(uint16_t, uint16_t);
INSTANTIATE_MATRIX(int16_t, int16_t);
INSTANTIATE_MATRIX(uint32_t, uint32_t);
INSTANTIATE_MATRIX(int32_t, int32_t);
INSTANTIATE_MATRIX(float, float);
INSTANTIATE_MATRIX(double, double);

template<class T1, class T2> Matrix<T1, T2>::Matrix() {
}

template<class T1, class T2> Matrix<T1, T2>::Matrix(const uint32_t& width,
		const uint32_t& height) {
	uint32_t* dims = new uint32_t[2];
	dims[0] = width;
	dims[1] = height;
	this->sample_model_ = new PixelInterleavedSampleModel<T1>(1, 2, dims);
	MemoryModel<T2>* memory_model = new MemoryModel<T2>(2,true,true,READ_WRITE);
	this->array_ = this->sample_model_->CreateArray(memory_model);
}

template<class T1, class T2> Matrix<T1, T2>::Matrix(const uint32_t& width,
		const uint32_t& height, MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model) {
	//this->array_ = new Array<T> (width, height, memory_model);
}

template<class T1, class T2> uint32_t Matrix<T1, T2>::GetWidth() {
	return this->array_->GetWidth();
}

template<class T1, class T2> uint32_t Matrix<T1, T2>::GetHeight() {
	return this->array_->GetHeight();
}

template<class T1, class T2> T1 Matrix<T1, T2>::Get(int x, int y) {
	return this->array_->Get(x, y);
}

template<class T1, class T2> void Matrix<T1, T2>::Set(T1 value, int x, int y) {
	this->array_->Set(value, x, y);
}

template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Transpose() {
	return *this;
}

template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Assign(const Matrix<T1, T2>& matrix) {
	return *this;
}

template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Add(const Matrix<T1, T2>& matrix) {
	return *this;
}

template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Add(const T1& value) {
	uint32_t size = this->GetHeight() * this->GetWidth();
	for(int i = 0;i < this->GetWidth() ; i++){
		for(int j=0;j< this->GetHeight();j++)
		{
			this->Set(this->Get(i,j)+value, i, j);
		}
	}
	return *this;
}

template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Multiply(const Matrix<T1, T2>& matrix) {
	return *this;
}
template<class T1, class T2> Matrix<T1, T2>& Matrix<T1, T2>::Multiply(const T1& value) {
	return *this;
}

template<class T1, class T2> T1 Matrix<T1, T2>::Det() {
	return 0;
}
}
