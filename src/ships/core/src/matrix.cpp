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

#define INSTANTIATE_DEVICE_MATRIX(T1, T2) \
	template class DeviceMatrix<T1, T2>; \
	template DeviceMatrix<T1, T2>::DeviceMatrix(); \
	template DeviceMatrix<T1, T2>::DeviceMatrix(const uint32_t& width, const uint32_t& height);
	//template DeviceMatrix<T1, T2>::DeviceMatrix(const uint32_t& width,const uint32_t& height, MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model); \
	template uint32_t DeviceMatrix<T1, T2>::GetWidth(); \
	template uint32_t DeviceMatrix<T1, T2>::GetHeight(); \
	template T1 DeviceMatrix<T1, T2>::Get(int x, int y); \
	template void DeviceMatrix<T1, T2>::Set(T1 value, int x, int y); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::DeviceMatrix<T1, T2>::Transpose(); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::Assign(const DeviceMatrix<T1, T2>& matrix); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::Add(const DeviceMatrix<T1, T2>& matrix); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::Add(const T1& value); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::Multiply(const DeviceMatrix<T1, T2>& matrix); \
	template DeviceMatrix<T1, T2>& DeviceMatrix<T1, T2>::Multiply(const T1& value); \
	template T1 DeviceMatrix<T1, T2>::Det();

INSTANTIATE_MATRIX(uint8_t, uint8_t);
INSTANTIATE_MATRIX(int8_t, int8_t);
INSTANTIATE_MATRIX(uint16_t, uint16_t);
INSTANTIATE_MATRIX(int16_t, int16_t);
INSTANTIATE_MATRIX(uint32_t, uint32_t);
INSTANTIATE_MATRIX(int32_t, int32_t);
INSTANTIATE_MATRIX(uint64_t, uint64_t);
INSTANTIATE_MATRIX(int64_t, int64_t);
INSTANTIATE_MATRIX(float, float);
INSTANTIATE_MATRIX(double, double);

INSTANTIATE_DEVICE_MATRIX(uint8_t, uint8_t);
INSTANTIATE_DEVICE_MATRIX(int8_t, int8_t);
INSTANTIATE_DEVICE_MATRIX(uint16_t, uint16_t);
INSTANTIATE_DEVICE_MATRIX(int16_t, int16_t);
INSTANTIATE_DEVICE_MATRIX(uint32_t, uint32_t);
INSTANTIATE_DEVICE_MATRIX(int32_t, int32_t);
INSTANTIATE_DEVICE_MATRIX(uint64_t, uint64_t);
INSTANTIATE_DEVICE_MATRIX(int64_t, int64_t);
INSTANTIATE_DEVICE_MATRIX(float, float);
INSTANTIATE_DEVICE_MATRIX(double, double);

//-----------------------------------------------------------------------------------------------------------------------
template<class T1, class T2> Matrix<T1, T2>::Matrix() {
	//TODO: unimplemented Matrix constructor.
}

template<class T1, class T2> Matrix<T1, T2>::Matrix(const uint32_t& width,
		const uint32_t& height) {
	this->width_ = width;
	this->height_ = height;
	uint32_t* dims = new uint32_t[2];
	dims[0] = width;
	dims[1] = height;
	this->sample_model_ = new PixelInterleavedSampleModel<T1>(1, 2, dims);
	MemoryModel<T2>* memory_model = new MemoryModel<T2>(2,true,true, sizeof(T2), READ_WRITE);
	this->array_ = this->sample_model_->CreateArray(memory_model);
}

template<class T1, class T2> Matrix<T1, T2>::Matrix(const uint32_t& width,
		const uint32_t& height, MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model) {
	this->width_ = width;
	this->height_ = height;
	//TODO: sample_model is unchecked for NULL value.
	this->sample_model_ = sample_model;
	this->array_ = this->sample_model_->CreateArray(memory_model);
}
//TODO: change this to sample_model_->Get
template<class T1, class T2> uint32_t Matrix<T1, T2>::GetWidth() {
	return this->width_;
}

template<class T1, class T2> uint32_t Matrix<T1, T2>::GetHeight() {
	return this->height_;
}

template<class T1, class T2> Array<T2>& Matrix<T1, T2>::GetArray(){
	return *this->array_;
}

template<class T1, class T2> T1 Matrix<T1, T2>::Get(int x, int y) {
	return this->sample_model_->GetSample(this->array_, x, y, 0);
}

template<class T1, class T2> void Matrix<T1, T2>::Set(T1 value, int x, int y) {
	this->sample_model_->SetSample(this->array_,value,x,y, 0);
	//this->array_->Set(value, x, y);
}
//TODO: this is just temporary solution to get a row. A good impl should use SampleModel instead.
template<class T1, class T2>  Vector<T1, T2>* Matrix<T1, T2>::GetRow(const uint32_t& row){
	T2* data = this->array_->GetData2D()[row];
}

template<class T1, class T2>  Vector<T1, T2>* Matrix<T1, T2>::GetColumn(const uint32_t& column){

}

//TODO: unimplemented matrix transposition.
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
//-----------------------------------------------------------------------------------------------------------------------
template<class T1, class T2> DeviceMatrix<T1,T2>::DeviceMatrix(){

}

template<class T1, class T2> DeviceMatrix<T1,T2>::DeviceMatrix(const uint32_t& width, const uint32_t& height){
	this->width_ = width;
	this->height_ = height;
	uint32_t* dims = new uint32_t[2];
	dims[0] = width;
	dims[1] = height;
	this->sample_model_ = new PixelInterleavedSampleModel<T1>(1, 2, dims);
	MemoryModel<T2>* memory_model = new DeviceMemoryModel<T2>(2,false,false, sizeof(T2), READ_WRITE);
	this->array_ = this->sample_model_->CreateArray(memory_model);
}
/*
template<class T1, class T2> DeviceMatrix<T1,T2>::DeviceMatrix(const uint32_t& width, const uint32_t& height, MemoryModel<
		T2>* memory_model,
		SampleModel<T1, T2>* sample_model){

}
*/
}
