/*
 * parallel_aco_ptv.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef PARALLEL_ACO_PTV_HPP_
#define PARALLEL_ACO_PTV_HPP_

namespace Sp{
class ParallelACOPTVEstimator: public ACOPTVEstimator{
public:
	virtual ~ParallelACOPTVEstimator(){}
	SHIPS_INLINE
	ParallelACOPTVEstimator(): Super(){
	}
	SHIPS_INLINE ParallelACOPTVEstimator(float alpha, float beta, float rho,
			float tau0, int cluster_size, int cluster_max, int cluster01_max,
			int num_particles, int num_ants, int num_loops) :
		Super(alpha, beta, rho, tau0, cluster_size, cluster_max, cluster01_max,
				num_particles, num_ants, num_loops) {

		this->tau_ = new Matrix<float, float> (num_particles, num_particles);
		this->N_ = new Vector<bool, bool> (num_particles);

		//int err;
		//this->context_ = DeviceManager::Instance().GetDefaultContext();
		//this->queue_ = cl::CommandQueue(context_,DeviceManager::Instance().GetDefaultDevice());
	}
	void Loop();
	/*
	 * @brief group particles into clusters.
	 */
	void Cluster();
	/*
	 * @brief this private function measure the distance between particles.
	 * @param first matrix
	 * @param second matrix
	 * @return the matrix represents distances between particles from f0 and f1
	 */
	Matrix<float, float>* Distances(Matrix<float, float>* f0, Matrix<float,
			float>* f1, bool relaxation = false);

	void AddFrame(Matrix<float,float>* frame) throw (InvalidContextException);
private:
	typedef ACOPTVEstimator Super;

	cl::CommandQueue queue_;
	cl::Context context_;
};
}

#endif /* PARALLEL_ACO_PTV_HPP_ */
