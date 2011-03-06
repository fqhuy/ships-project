/*
 * motion_estimator.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef MOTION_ESTIMATOR_HPP_
#define MOTION_ESTIMATOR_HPP_

namespace Sp {
enum MotionEstimatorTypes {
	DIRECT, INDIRECT
};
class MotionEstimator {
public:
	virtual ~MotionEstimator(){}
	virtual Matrix<int,int>* Estimate()=0;

	virtual void AddFrame(Matrix<float, float>* matrix){
		frames_.push_back(matrix);
	}
	virtual void RemoveFrame(const uint32_t& index){
		//TODO: unimplemented
		//frames_.erase(index);
	}
protected:
	SHIPS_INLINE
	MotionEstimator(){}
	MotionEstimatorTypes type_;

	std::vector<Matrix<float, float>* > frames_;
};
}

#endif /* MOTION_ESTIMATOR_HPP_ */
