/*
 * host_aco_ptv.hpp
 *
 *  Created on: Mar 9, 2011
 *      Author: fqhuy
 */

#ifndef HOST_ACO_PTV_HPP_
#define HOST_ACO_PTV_HPP_

namespace Sp {
class HostACOPTVEstimator: public ACOPTVEstimator {
public:
	SHIPS_INLINE
	HostACOPTVEstimator() :	Super() {
	}

	SHIPS_INLINE
	HostACOPTVEstimator(float alpha, float beta, float rho,
			float tau0, int cluster_size, int cluster_max, int cluster01_max,
			int num_particles, int num_ants, int num_loops) :
		Super(alpha, beta, rho, tau0, cluster_size, cluster_max, cluster01_max,
				num_particles, num_ants, num_loops) {

		tau_ = new HostMatrix<float, float> (num_particles, num_particles);
		this->N_ = new HostVector<bool, bool> (this->num_particles_);
	}

	virtual ~HostACOPTVEstimator() {
	}
	/*
	 * @brief calculate the sum of relaxation length between particle p0 and particle p1
	 * @param p0 first particle
	 * @param p1 second particle
	 */
	float RelaxationLength(const int& p0, const int& p1); //private
	/*
	 * @brief choose the next node to go
	 * @param current_pos the current particle
	 * @param probability return the probability of traveling from current node to selected node
	 * @return the selected node.
	 */
	int Next(int current_pos, float* probability, bool relaxation);
	/*
	 * @brief main loop
	 */
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
			float>* f1,Matrix<float, float>* result=NULL,  bool relaxation = false);
	/*
	 * @brief this function sort the elements in the same row increasingly.
	 * @param matrix the matrix to sort.
	 */
	void Sort(Matrix<float, float>* matrix, Matrix<int, int>* indices);
	/*
	 * @brief simple distance calculation.
	 */
	SHIPS_INLINE
	float Distance(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

private:
	typedef ACOPTVEstimator Super;
};
}

#endif /* HOST_ACO_PTV_HPP_ */
