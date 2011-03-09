/*
 * ACOPTVEstimator.hpp
 *
 *  Created on: Mar 3, 2011
 *      Author: fqhuy
 */

#ifndef ACO_PTV_HPP_
#define ACO_PTV_HPP_

namespace Sp {
class ACOPTVEstimator: public MotionEstimator {
public:
	SHIPS_INLINE ACOPTVEstimator() {

	}
	SHIPS_INLINE ACOPTVEstimator(float alpha, float beta, float rho,
			float tau0, int cluster_size, int cluster_max, int cluster01_max, int num_particles, int num_ants,
			int num_loops) :
		alpha_(alpha), beta_(beta), rho_(rho), tau0_(tau0), cluster_size_(
				cluster_size), cluster_min_(cluster_size), cluster_max_(cluster_max), cluster01_max_(cluster01_max), num_particles_(num_particles), num_loops_(
				num_loops), num_ants_(num_ants), f0_(NULL), f1_(NULL), cf0_(
				NULL), cf1_(NULL), cf0f1_(NULL), tau_(NULL), N_(NULL),
				clusters0_(NULL), clusters1_(NULL), clusters01_(NULL), result_(NULL), Super() {
		tau_ = new HostMatrix<float, float> (num_particles, num_particles);
	}

	virtual ~ACOPTVEstimator() {
		if (tau_)
			delete tau_;
		//if (f0_)
		//	delete f0_;
		//if (f1_)
		//	delete f1_;
		if (cf0_)
			delete cf0_;
		if (cf1_)
			delete cf1_;
		if (cf0f1_)
			delete cf0f1_;
		if (N_)
			delete N_;
		if (clusters0_)
			delete clusters0_;
		if (clusters1_)
			delete clusters1_;
		if(clusters01_)
			delete clusters01_;
	}

	Matrix<int, int>* Estimate();
	/*
	 * @brief calculate the sum of relaxation lengths between frame f0 and frame f1
	 * @param f0 array of particle's indices from frame 0
	 *
	 */
	float RelaxationLength(const int& p0, const int& p1); //private
	/*
	 * @brief update matrix Tau(i,j)
	 */
	//void Update();
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
			float>* f1, bool relaxation = false);
	/*
	 * @brief this function sort the elements in the same row increasingly.
	 * @param matrix the matrix to sort.
	 */
	void Sort(Matrix<float, float>* matrix, Matrix<int, int>* indices);
	/*
	 * @brief sort the numbers array using Quick Sort algorithm.
	 */
	template<class T> void QuickSort(T numbers[], int indices[], int left,
			int right);
	/*
	 * @brief simple distance calculation.
	 */
	SHIPS_INLINE
	float Distance(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	SHIPS_INLINE
	Matrix<float,float>* Cf0f1(){
		return cf0f1_;
	}

	SHIPS_INLINE
	Matrix<int,int>* Clusters01(){
		return clusters01_;
	}
	SHIPS_INLINE
	Matrix<int,int>* Clusters0(){
		return clusters0_;
	}
	SHIPS_INLINE
	Matrix<int,int>* Clusters1(){
		return clusters1_;
	}
private:
	/*
	 * f0_ frame 0
	 * f1_ frame 1
	 * cf0_ the matrix represents distances between each point of frame 0 to other points in the same frame
	 * cf1_ the matrix represents distances between each point of frame 1 to other points in the same frame
	 * cf0f1_ the matrix represents distances between each point of frame 0 to each points of frame 1
	 * tau_ this matrix stores the amount of pheromone on the path connecting each point of frame 0, to other points in frame 1
	 */
	Matrix<float, float>* f0_, *f1_, *cf0_, *cf1_, *cf0f1_, *tau_;
	/*
	 * @brief is a list of particles. false = visited, true = unvisited.
	 */
	Vector<bool, bool>* N_;
	/*
	 * clusters0_ matrix of clusters in frame 0. every row is a list of particles sorted decreasingly by distance from particle[row id] to particle[clusters0_[column id]]
	 * clusters1_ matrix of clusters in frame 1.
	 * result_ num_particles_ x 2 matrix. each column is a list of particle id.
	 */
	Matrix<int, int>* clusters0_, *clusters1_, *clusters01_, *result_;
	float alpha_, beta_, rho_, tau0_;
	int num_ants_, num_particles_, num_loops_, cluster_size_, cluster_min_, cluster_max_, cluster01_max_;
	typedef MotionEstimator Super;
};
}

#endif /* ACO_PTV_HPP_ */
