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

#ifndef __SHIPS_CORE_VECTOR__
#define __SHIPS_CORE_VECTOR__

#include <boost/numeric/ublas/vector.hpp>

namespace Sp {

    template<class T> class VectorAdapter {
        public:
            typedef VectorAdapter<T> SelfType;
            typedef T ValueType;
            typedef T& Reference;
            typedef const T& ConstReference;
            typedef T* Pointer;

            VectorAdapter();
            virtual ~VectorAdapter() {};
            virtual Reference At ( const int& index ) =0;
            virtual void Resize ( const int& size, const bool& preserve = true ) =0;
            virtual void Insert ( const int& pos, ConstReference element ) =0;
            virtual void Remove ( const int& index ) =0;
            //---------- GET ----------//
            virtual int& GetSize() =0;
            //---------- GET ----------//
        protected:
        private:
    };

    template<class T, class A> class HostVectorAdapter:
                public VectorAdapter<T> {
        public:
            typedef HostVectorAdapter<T,A> SelfType;
            typedef T ValueType;
            typedef T& Reference;
            typedef const T& ConstReference;
            typedef T* Pointer;
            typedef A ArrayType;
            typedef boost::numeric::ublas::vector<T,A> VectorType;

            HostVectorAdapter();
            HostVectorAdapter ( const int& size );

            virtual ~HostVectorAdapter() {};

            Reference At ( const int& index );
            void Insert ( const int& pos, ConstReference element );
            void Resize ( const int& size,const bool& preserve = true );
            void Remove ( const int& index );

            //---------- GET ----------//
            VectorType GetData() {
                return data_;
            }
            //---------- GET ----------//
        protected:

        private:
            VectorType data_;
    };

}
#endif // __SHIPS_CORE_VECTOR__
