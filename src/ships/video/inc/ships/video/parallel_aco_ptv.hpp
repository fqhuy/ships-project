/*
 * parallel_aco_ptv.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef PARALLEL_ACO_PTV_HPP_
#define PARALLEL_ACO_PTV_HPP_

namespace Sp{
class ParallelACOPTVEstimator: public MotionEstimator {
public:
	SHIPS_INLINE
	ParallelACOPTVEstimator(const uint32_t& num_frames, Matrix<float,float>* frames): Super(num_frames,frames){
	}

	Matrix<float,float>* Estimate();
private:
	Vector<int,int>* clusters_;
	typedef MotionEstimator Super;

	void Cluster();

};
}

#endif /* PARALLEL_ACO_PTV_HPP_ */
