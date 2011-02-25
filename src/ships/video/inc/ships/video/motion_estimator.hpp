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

	virtual Matrix<float, float>* Estimate()=0;

	virtual void AddFrame(Matrix<float, float>* matrix)=0;
	virtual void RemoveFrame(const uint32_t& index)=0;
protected:
	SHIPS_INLINE
	MotionEstimator(const uint32_t& num_frames, Matrix<float,
			float>* frames) :
		num_frames_(num_frames), frames_(frames) {
	}
	MotionEstimatorTypes type_;
	uint32_t num_frames_;
	Matrix<float, float>* frames_;
};
}

#endif /* MOTION_ESTIMATOR_HPP_ */
