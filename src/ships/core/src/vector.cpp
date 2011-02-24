/*
 <one line to give the program's name and a brief idea of what it does.>
 Copyright (C) <year>  <name of author>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

#include <ships/core/core.hpp>

namespace Sp {
#define INSTANTIATE_VECTOR(T1, T2) \
	template class Vector<T1, T2>; \
	template Vector<T1, T2>::Vector(const uint32_t& size); \
	template Vector<T1, T2>::Vector(const uint32_t& size, MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model); \
	template void Vector<T1, T2>::Resize(const uint32_t& size, const bool& preserve ); \
	template void Vector<T1, T2>::Insert(const int& pos, const T1& element); \
	template void Vector<T1, T2>::Remove(const int& index); \
	template Vector<T1, T2> Vector<T1, T2>::Cross(const Vector<T1, T2>& vector); \
	template Vector<T1, T2> Vector<T1, T2>::Mul(const Vector<T1, T2>& vector);
INSTANTIATE_VECTOR(int8_t,int8_t)
INSTANTIATE_VECTOR(int16_t,int16_t)
INSTANTIATE_VECTOR(int32_t,int32_t)
INSTANTIATE_VECTOR(int64_t,int64_t)
INSTANTIATE_VECTOR(uint8_t,uint8_t)
INSTANTIATE_VECTOR(uint16_t,uint16_t)
INSTANTIATE_VECTOR(uint32_t,uint32_t)
INSTANTIATE_VECTOR(uint64_t,uint64_t)
INSTANTIATE_VECTOR(float,float)
INSTANTIATE_VECTOR(double,double)

template<class T1, class T2> Vector<T1, T2>::Vector(const uint32_t& size) {
	uint32_t s = size;
	MemoryModel<T2>* mm = new MemoryModel<T2>(1,true,true,sizeof(T2),READ_WRITE);
	Sp::SampleModel<T1,T2>* sm = new Sp::PixelInterleavedSampleModel<T1>(1,1,&s);
	this->sample_model_ = sm;
	this->size_ = size;
	this->array_ = sm->CreateArray(mm);
}

template<class T1, class T2> Vector<T1, T2>::Vector(const uint32_t& size,
		MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model) {
	this->size_ = size;
	this->sample_model_ = sample_model;
	this->array_ = sample_model->CreateArray(memory_model);
}

template<class T1, class T2> void Vector<T1, T2>::Resize(const uint32_t& size,
		const bool& preserve) {

}
template<class T1, class T2> void Vector<T1, T2>::Insert(const int& pos,
		const T1& element) {

}
template<class T1, class T2> void Vector<T1, T2>::Remove(const int& index) {

}
template<class T1, class T2> Vector<T1, T2> Vector<T1, T2>::Cross(const Vector<
		T1, T2>& vector){

}
template<class T1, class T2> Vector<T1, T2> Vector<T1, T2>::Mul(const Vector<
		T1, T2>& vector){

}
}

