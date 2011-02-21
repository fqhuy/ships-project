/*
 * geometry.hpp
 *
 *  Created on: Feb 13, 2011
 *      Author: fqhuy
 */

#ifndef GEOMETRY_2D_HPP_
#define GEOMETRY_2D_HPP_

namespace Sp{

template<class T> class Shape{

};

template<class T> class Point2D{
public:
	typedef T ValueType;
	SHIPS_INLINE Point2D(){x_=0;y_=0;}
	SHIPS_INLINE Point2D(ValueType x, ValueType y): x_(x), y_(y){}
	SHIPS_INLINE Point2D(const Point2D& p){x_ = p.GetX();y_ = p.GetY();}

	SHIPS_INLINE ValueType GetX(){return x_;}
	SHIPS_INLINE ValueType GetY(){return y_;}

	SHIPS_INLINE void Set(ValueType x, ValueType y){x_ = x; y_ = y;}

	SHIPS_INLINE bool Equals(const Point2D& p){return p.GetX()==x_ && p.GetY()==y_ ? true : false ;}
private:
	ValueType x_;
	ValueType y_;
};

template<class T> class RectangularShape: public Shape <T>{
public:
	typedef T ValueType;
	SHIPS_INLINE RectangularShape(){x_=0;y_=0;width_=0;height_=0;}
	SHIPS_INLINE RectangularShape(ValueType x, ValueType y, ValueType width, ValueType height): x_(x), y_(y), width_(width), height_(height){}

	virtual ~RectangularShape() = 0;

	virtual ValueType GetX(){return x_;}
	virtual ValueType GetY(){return y_;}
	virtual ValueType GetWidth(){return width_;}
	virtual ValueType GetHeight(){return height_;}

	SHIPS_INLINE virtual void Set(ValueType x, ValueType y, ValueType w, ValueType h){x_ = x; y_ = y; width_ = w ; height_ = h;}
protected:
	ValueType x_;
	ValueType y_;
	ValueType width_;
	ValueType height_;
};

template<class T> class Rectangle2D: public RectangularShape<T> {
public:
	typedef T ValueType;

	SHIPS_INLINE Rectangle2D(): RectangularShape<T>() {}
	SHIPS_INLINE Rectangle2D(ValueType x, ValueType y, ValueType width, ValueType height): RectangularShape<T>(x,y,width,height) {}
	SHIPS_INLINE Rectangle2D(const Rectangle2D& r){}
};

template<class T> class Ellipse2D: public RectangularShape<T> {

};

template<class T> class Arc2D: public RectangularShape<T>{

};


}

#endif /* GEOMETRY_HPP_ */
