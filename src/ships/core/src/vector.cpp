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

namespace Sp{
  
  template<class T, class A> HostVectorAdapter<T,A>::HostVectorAdapter(const int& size){
    this->data_.resize(size);
  }
  
  template<class T, class A> void HostVectorAdapter<T,A>::Insert(const int& pos, HostVectorAdapter<T,A>::ConstReference element ){
    this->data_.insert_element(pos, element);
  }
  
  template<class T, class A> void HostVectorAdapter<T,A>::Resize(const int& size,const bool& preserve){
    this->data_.resize(size, preserve);
  }
  
  template<class T, class A> void HostVectorAdapter<T,A>::Remove(const int& index){
    this->data_.remove(index);
  }  
  template<class T, class A> T& HostVectorAdapter<T,A>::At(const int& index){
    this->data_(index);
  }  
}

