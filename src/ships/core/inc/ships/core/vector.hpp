/*
 <one line to give the program's name and a brief idea of what it does.>
 Copyright (C) <2011>  <Phan Quoc Huy>

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

#ifndef __SHIPS_CORE_VECTOR__
#define __SHIPS_CORE_VECTOR__

namespace Sp {
/**
 * This class is an impl of mathematical vector.
 * @author Phan Quoc Huy
 */
template<class T1, class T2> class Vector {
public:
	typedef Vector<T1, T2> SelfType;
	typedef T1 ValueType1;
	typedef T2 ValueType2;
	typedef T1& Reference;
	typedef const T1& ConstReference;
	typedef T1* Pointer;

	//TODO: Need copy constructor here.
	Vector(const uint32_t& size);
	Vector(const uint32_t& size, MemoryModel<ValueType2>* memory_model, SampleModel<ValueType1, ValueType2>* sample_model);
	virtual ~Vector() {
		if(array_)
			delete array_;
		if(sample_model_)
			delete sample_model_;
	}
	SHIPS_INLINE
	virtual void Set(ValueType1 value, const int& index){
		sample_model_->SetSample(array_,value,index,0);
	}
	virtual void Resize(const uint32_t& size, const bool& preserve = true);
	virtual void Insert(const int& pos, ConstReference element);
	virtual void Remove(const int& index);
	virtual SelfType Cross(const SelfType& vector);
	virtual SelfType Mul(const SelfType& vector);
	//---------- GET ----------//
	SHIPS_INLINE
	virtual ValueType1 Get(const int& index){
		return sample_model_->GetSample(array_,index,0);
	}
	SHIPS_INLINE
	virtual Array<T2>* GetArray(){
		return array_;
	}
	SHIPS_INLINE
	virtual uint32_t& GetSize(){
		return size_;
	}
	//---------- GET ----------//
protected:
	uint32_t size_;
	Array<T2>* array_;
	SampleModel<T1, T2>* sample_model_;
private:
};

template<class T1, class T2> class HostVector: public Vector<T1, T2> {
public:
	typedef HostVector<T1, T2> SelfType;
	typedef T1 ValueType;
	typedef T1& Reference;
	typedef const T1& ConstReference;
	typedef T1* Pointer;

	HostVector();
	HostVector(const int& size);

	virtual ~HostVector() {
	}

	void Set(ValueType value, const uint32_t& index);
	void Insert(const int& pos, ConstReference element);
	void Resize(const int& size, const bool& preserve = true);
	void Remove(const int& index);

	//---------- GET ----------//
	ValueType Get(const int& index);
	//---------- GET ----------//
protected:
private:
};

}
#endif // __SHIPS_CORE_VECTOR__
