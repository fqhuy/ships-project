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

template<class T> HostPixel<T>::HostPixel(const T& r, const T& g, const T& b,
		const T& a) {
	data_[0] = r;
	data_[1] = g;
	data_[2] = b;
	data_[3] = a;
}

/*
template<class T> T HostPixel<T>::At(const int& index) {
	return data_[index];
}
*/
//template<class T> Pixel<T>* HostPixel<T>::Add(const Pixel<T>& value) {
	//data_[0] = saturate_cast(data_[0] + value.At(0));
	//data_[0] = saturate_cast(data_[1] + value.At(1));
	//data_[0] = saturate_cast(data_[2] + value.At(2));
	//data_[0] = saturate_cast(data_[3] + value.At(3));

//	return this;
//}
/*
template<class T> Pixel<T>* HostPixel<T>::Assign(const Pixel<T>& value) {
	//data_[0] = value.At(0);
	//data_[1] = value.At(1);
	//data_[2] = value.At(2);
	//data_[3] = value.At(3);

	return this;
}
*/
}
