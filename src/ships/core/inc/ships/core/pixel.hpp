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

#ifndef __SHIPS_CORE_PIXEL__
#define __SHIPS_CORE_PIXEL__

namespace Sp {

    template<typename T> static inline T saturate_cast ( unsigned char v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( signed char v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( unsigned short v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( short v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( unsigned v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( int v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( float v ) {
        return T ( v );
    }
    template<typename T> static inline T saturate_cast ( double v ) {
        return T ( v );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( signed char v ) {
        return ( unsigned char ) std::max ( ( int ) v, 0 );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( unsigned short v ) {
        return ( unsigned char ) std::min ( ( unsigned ) v, ( unsigned ) UCHAR_MAX );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( int v ) {
        return ( unsigned char ) ( ( unsigned ) v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0 );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( short v ) {
        return saturate_cast<unsigned char> ( ( int ) v );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( unsigned v ) {
        return ( unsigned char ) std::min ( v, ( unsigned ) UCHAR_MAX );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( float v ) {
        int iv = round ( v );
        return saturate_cast<unsigned char> ( iv );
    }
    template<> inline unsigned char saturate_cast<unsigned char> ( double v ) {
        int iv = round ( v );
        return saturate_cast<unsigned char> ( iv );
    }

    template<> inline signed char saturate_cast<signed char> ( unsigned char v ) {
        return ( signed char ) std::min ( ( int ) v, SCHAR_MAX );
    }
    template<> inline signed char saturate_cast<signed char> ( unsigned short v ) {
        return ( signed char ) std::min ( ( unsigned ) v, ( unsigned ) SCHAR_MAX );
    }
    template<> inline signed char saturate_cast<signed char> ( int v ) {
        return ( signed char ) ( ( unsigned ) ( v-SCHAR_MIN ) <= ( unsigned ) UCHAR_MAX ?
                                 v : v > 0 ? SCHAR_MAX : SCHAR_MIN );
    }
    template<> inline signed char saturate_cast<signed char> ( short v ) {
        return saturate_cast<signed char> ( ( int ) v );
    }
    template<> inline signed char saturate_cast<signed char> ( unsigned v ) {
        return ( signed char ) std::min ( v, ( unsigned ) SCHAR_MAX );
    }

    template<> inline signed char saturate_cast<signed char> ( float v ) {
        int iv = round ( v );
        return saturate_cast<signed char> ( iv );
    }
    template<> inline signed char saturate_cast<signed char> ( double v ) {
        int iv = round ( v );
        return saturate_cast<signed char> ( iv );
    }

    template<> inline unsigned short saturate_cast<unsigned short> ( signed char v ) {
        return ( unsigned short ) std::max ( ( int ) v, 0 );
    }
    template<> inline unsigned short saturate_cast<unsigned short> ( short v ) {
        return ( unsigned short ) std::max ( ( int ) v, 0 );
    }
    template<> inline unsigned short saturate_cast<unsigned short> ( int v ) {
        return ( unsigned short ) ( ( unsigned ) v <= ( unsigned ) USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0 );
    }
    template<> inline unsigned short saturate_cast<unsigned short> ( unsigned v ) {
        return ( unsigned short ) std::min ( v, ( unsigned ) USHRT_MAX );
    }
    template<> inline unsigned short saturate_cast<unsigned short> ( float v ) {
        int iv = round ( v );
        return saturate_cast<unsigned short> ( iv );
    }
    template<> inline unsigned short saturate_cast<unsigned short> ( double v ) {
        int iv = round ( v );
        return saturate_cast<unsigned short> ( iv );
    }

    template<> inline short saturate_cast<short> ( unsigned short v ) {
        return ( short ) std::min ( ( int ) v, SHRT_MAX );
    }
    template<> inline short saturate_cast<short> ( int v ) {
        return ( short ) ( ( unsigned ) ( v - SHRT_MIN ) <= ( unsigned ) USHRT_MAX ?
                           v : v > 0 ? SHRT_MAX : SHRT_MIN );
    }
    template<> inline short saturate_cast<short> ( unsigned v ) {
        return ( short ) std::min ( v, ( unsigned ) SHRT_MAX );
    }
    template<> inline short saturate_cast<short> ( float v ) {
        int iv = round ( v );
        return saturate_cast<short> ( iv );
    }
    template<> inline short saturate_cast<short> ( double v ) {
        int iv = round ( v );
        return saturate_cast<short> ( iv );
    }

    template<> inline int saturate_cast<int> ( float v ) {
        return round ( v );
    }
    template<> inline int saturate_cast<int> ( double v ) {
        return round ( v );
    }

// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
    template<> inline unsigned saturate_cast<unsigned> ( float v ) {
        return round ( v );
    }
    template<> inline unsigned saturate_cast<unsigned> ( double v ) {
        return round ( v );
    }

    template<class T>
    class Pixel {
        public:

            typedef Pixel<T> SelfType;
            typedef Pixel<T>& ConstSelfType;
            typedef T ValueType;
            typedef T &Reference;

            //Pixel();
            virtual ~Pixel() {};

            virtual SelfType* Add ( const SelfType& value ) =0;
            //virtual SelfType Multiply ( const SelfType& value ) =0;
            //virtual SelfType Substract ( const SelfType& value ) =0;
            //virtual SelfType And ( const SelfType& value ) =0;
            //virtual ValueType At ( const int& index)= 0;
            //virtual SelfType* Assign( const SelfType& value) = 0;
        protected:

        private:

            //int num_elements_;
    };

    template<class T> class HostPixel:
                public Pixel<T> {
        public:
            typedef HostPixel<T> SelfType;
            typedef T ValueType;

            typedef T &Reference;
            typedef const T& ConstReference;

            HostPixel(){}
            HostPixel ( ConstReference r,ConstReference g,ConstReference b,ConstReference a );

            virtual ~HostPixel() {}

            Pixel<T>* Add ( const Pixel<T>& value );//{}
            //ValueType At ( const int& index );
            //Pixel<T>* Assign( const Pixel<T>& value);
        protected:
            ValueType data_[4];
        private:
    };
                template<class T> Pixel<T>* HostPixel<T>::Add ( const Pixel<T>& value ){}
}
#endif
