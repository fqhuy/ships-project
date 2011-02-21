/*
 * geometry_3d.hpp
 *
 *  Created on: Feb 13, 2011
 *      Author: fqhuy
 */

#ifndef GEOMETRY_3D_HPP_
#define GEOMETRY_3D_HPP_

namespace Sp{

template<class T> class Point3D{
	typedef T ValueType;
	SHIPS_INLINE Point3D(){x_=0;y_=0;z_=0;}
	SHIPS_INLINE Point3D(ValueType x, ValueType y, ValueType z){x_ = x; y_ = y;z_ = z;}
	SHIPS_INLINE Point3D(const Point3D& p){x_ = p.GetX();y_ = p.GetY();z_ = p.GetZ();}

	SHIPS_INLINE ValueType GetX(){return x_;}
	SHIPS_INLINE ValueType GetY(){return y_;}
	SHIPS_INLINE ValueType GetZ(){return z_;}

	SHIPS_INLINE void Set(ValueType x, ValueType y, ValueType z){x_ = x; y_ = y; z_ = z;}

	SHIPS_INLINE bool Equals(const Point3D& p){return p.GetX()==x_ && p.GetY()==y_ && p.GetZ()==z_? true : false ;}
private:
	ValueType x_;
	ValueType y_;
	ValueType z_;
};

}

#endif /* GEOMETRY_3D_HPP_ */
