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
	virtual ~ParallelACOPTVEstimator(){}
	SHIPS_INLINE
	ParallelACOPTVEstimator(): Super(){
	}

	Matrix<int,int>* Estimate();
private:
	Vector<int,int>* clusters_;
	typedef MotionEstimator Super;

	void Cluster();
	void Distance();
};
}

#endif /* PARALLEL_ACO_PTV_HPP_ */
