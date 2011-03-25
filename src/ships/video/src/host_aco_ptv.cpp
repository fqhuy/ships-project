/*
 * host_aco_ptv.cpp
 *
 *  Created on: Mar 9, 2011
 *      Author: fqhuy
 */
#include <ships/video/video.hpp>

namespace Sp{

float HostACOPTVEstimator::RelaxationLength(const int& p0, const int& p1) {
	float length = 0;
	float dx1, dx2, dy1, dy2, rx1, rx2, ry1, ry2, min, diff;
	int r1, r2, temp, mid;
	bool tabu[this->cluster_size_];
	for (int i = 0; i < this->cluster_size_; i++)
		tabu[i] = true;
	//location of central particles.
	dx1 = this->f0_->Get(p0, 0);
	dy1 = this->f0_->Get(p0, 1);
	dx2 = this->f1_->Get(p1, 0);
	dy2 = this->f1_->Get(p1, 1);

	for (int i = 0; i < this->cluster_size_; i++) {
		//get the ID of (i+1)-th particle in the cluster 0.
		r1 = this->clusters0_->Get(p0, i + 1);
		rx1 = this->f0_->Get(r1, 0);
		ry1 = this->f0_->Get(r1, 1);

		min = INT_MAX;
		//find within second cluster the closest particle
		//TODO: possible to speedup here
		for (int k = 0; k < this->cluster_size_; k++) {
			if (tabu[k] == true) {
				temp = this->clusters1_->Get(p1, k + 1);
				rx2 = this->f1_->Get(temp, 0);
				ry2 = this->f1_->Get(temp, 1);
				diff = sqrt((dx2 + rx1 - dx1 - rx2) * (dx2 + rx1 - dx1 - rx2)
						+ (dy2 + ry1 - dy1 - ry2) * (dy2 + ry1 - dy1 - ry2));

				if (diff < min ) {
					min = diff;
					r2 = temp;
					mid = k;
				}
			}
		}
		tabu[mid] = false;
		length += min;
	}
	length *= ((float) this->cluster_min_ / this->cluster_size_);
	return length;
}

int HostACOPTVEstimator::Next(int current_pos, float* probability, bool relaxation) {
	float prob = 1, eta = 0, denominator = 0;
	int selected = -1, cup;
	//finding Eta
	/*
	 for (int i = 0; i < this->num_particles_; i++) {
	 if (N_->Get(i) == 1) {
	 //currently eta is calculated as reciprocal of the distance between two particles.
	 //eta = 1 / this->cf0f1_->Get(current_pos, i);
	 // comment out bellow line to use relaxation length. usually, we use conventional length for the 1st iteration. ***
	 if (relaxation)
	 eta = 1 / this->RelaxationLength(current_pos, i);
	 else
	 eta = 1 / this->cf0f1_->Get(current_pos, i);

	 denominator += (pow(this->tau_->Get(current_pos, i), this->alpha_)
	 * pow(eta, this->beta_));
	 }
	 }
	 */
	//loop through all unvisited particles in second frame to find one with highest probability.
	float max = 0, tauij;

	//this loop can be speeded up using the ordered matrix clusters01_.
	for (int i = 0; i < this->cluster01_max_/*this->num_particles*/; i++) {
		cup = this->clusters01_->Get(current_pos, i);
		//cup = i;
		if (N_->Get(cup) == 1) {
			if (relaxation)
				eta = 1 / (this->RelaxationLength(current_pos, cup));
			else
				eta = 1 / this->cf0f1_->Get(current_pos, cup);
			//temporarily
			tauij = this->tau_->Get(current_pos, cup);
			prob = (pow(tauij, this->alpha_) * pow(eta, this->beta_));// denominator;
			if (prob > max) {
				max = prob;
				selected = cup;
			}
		}
	}
	if (max == 0) {
		//LOG4CXX_WARN(Sp::video_logger, "Can not find any sufficient particle, consider changing the number of particles to look at..");
		return -1;
	}

	//if probability is not NULL, set the value to prob
	if (probability != NULL)
		(*probability) = max;
	return selected;
}

void HostACOPTVEstimator::Loop() {
	Random rd;
	int i, j, k, h, next;
	float tau, delta_tau, l, Lk, bestLk = INT_MAX, lbestLk;
	//final result
	int *rf0, *rf1, *lrf0, *lrf1, in=1;

	int np = this->num_particles_;
	int na = this->num_ants_;
	int nl = this->num_loops_;

	for (i = 0; i < nl; i++) { //for nl number of loops.
		rd.SRand();
		//this->tau_->Init(this->tau0_);
		if (this->cluster_size_ < this->cluster_max_)
			this->cluster_size_++;

		//Tk0[ant][particle id]
		int Tk0[na][np];
		int Tk1[na][np];

		for (j = 0; j < na; j++) { //for every ant.
			this->N_->Init(1);
			//init with index value
			for (k = 0; k < np; k++)
				Tk0[j][k] = k;

			//randomize the itinerary of the ants by randomly exchanging values of elements.

			int temp, a, b;

			for (k = 0; k < np * 2; k++) { //for every particle *****
				a = rd.Next<int> (0, np - 1);
				b = rd.Next<int> (0, np - 1);
				temp = Tk0[j][a];
				Tk0[j][a] = Tk0[j][b];
				Tk0[j][b] = temp;
			}

			for (k = 0; k < np; k++) { //for every particle
				if (i < 1)
					next = this->Next(Tk0[j][k], NULL, false);
				else
					next = this->Next(Tk0[j][k], NULL, true);

				if (next == -1) {
					next = 0;
					//break;
				}
				//update the route
				Tk1[j][k] = next;
				//mark this particle as visited
				this->N_->Set(0, next);
			}
		}

		lbestLk = INT_MAX;
		for (k = 0; k < np; k++)
			for (h = 0; h < np; h++) {
				tau = this->tau_->Get(k, h);
				this->tau_->Set((1 - this->rho_) * tau, k, h);
			}

		//Update Tau matrix for every ant.
		for (j = 0; j < na; j++) {
			Lk = 0;
			//Find Lk
			for (k = 0; k < np; k++)
				//Lk += this->cf0f1_->Get(Tk0[j][k], Tk1[j][k]);
			// comment out to use relaxation length.
			Lk += this->RelaxationLength(Tk0[j][k], Tk1[j][k]);

			if (Lk < bestLk) {
				bestLk = Lk;
				rf0 = Tk0[j];
				rf1 = Tk1[j];
			}

			if (Lk < lbestLk) {
				lbestLk = Lk;
				lrf0 = Tk0[j];
				lrf1 = Tk1[j];
			}

			//find delta tau

			 delta_tau = 1 / Lk;
			 //update pheromone on each portion

			 for (k = 0; k < np; k++) {
			 tau = this->tau_->Get(Tk0[j][k], Tk1[j][k]);
			 this->tau_->Set(tau + delta_tau, Tk0[j][k], Tk1[j][k]);
			 }

		}
		//update pheromone on each portion using best Lk

		delta_tau = 1 / lbestLk;
		for (k = 0; k < np; k++) {
			tau = this->tau_->Get(lrf0[k], lrf1[k]);
			this->tau_->Set(tau + delta_tau, lrf0[k], lrf1[k]);
		}

		if (i == (nl - 1)) {
			this->result_ = new HostMatrix<int, int> (2, np);

			for (k = 0; k < np; k++) {
				this->result_->Set(lrf0[k], k, 0);//(rf0[i], i, 0);
				this->result_->Set(lrf1[k], k, 1);//(rf1[i], i, 1);
			}
		}
	}

}

void HostACOPTVEstimator::Sort(Matrix<float, float>* matrix,
		Matrix<int, int>* indices) {
	float* row = NULL;
	int* irow = NULL;
	Matrix<float, float>* clone = matrix->Clone();

	for (int i = 0; i < matrix->GetHeight(); i++) {
		row = clone->GetArray().GetData2D()[i];
		irow = indices->GetArray().GetData2D()[i];
		ArrayUtils::QuickSort<float>(row, irow, 0, clone->GetWidth() - 1);
	}
}

Matrix<float, float>* HostACOPTVEstimator::Distances(Matrix<float, float>* f0,
		Matrix<float, float>* f1,Matrix<float, float>* result,  bool relaxation) {

	if (f0 == NULL || f1 == NULL)
		return NULL;

	if (f0->GetHeight() != f1->GetHeight() || f0->GetWidth() != f1->GetWidth())
		return NULL;

	const int w = f0->GetWidth();
	const int h = f1->GetHeight();
	const int half = h / 2 + 1;

	Matrix<float, float>* result_ = new HostMatrix<float, float> (h, h);
	result_->Zeros();

	float temp;

	float x1, x2, y1, y2;
	int end = 0;
	for (int i = 0; i < h; i++) {
		x1 = f0->Get(i, 0);
		y1 = f0->Get(i, 1);
		for (int j = end; j < h; j++) {
			x2 = f1->Get(j, 0);
			y2 = f1->Get(j, 1);

			if (relaxation)
				temp = this->RelaxationLength(i, j);
			else
				temp = this->Distance(x1, y1, x2, y2);
			result_->Set(temp, i, j);
		}
	}

	return result_;
}

void HostACOPTVEstimator::Cluster() {
	int np = this->num_particles_;
	this->clusters0_ = new HostMatrix<int, int> (np, np);
	this->clusters1_ = new HostMatrix<int, int> (np, np);
	this->clusters01_ = new HostMatrix<int, int> (np, np);
	//initialize the index matrices.
	for (int i = 0; i < np; i++) //for each row
		for (int j = 0; j < np; j++) { //for each element in a row
			clusters0_->Set(j, i, j);
			clusters1_->Set(j, i, j);
			clusters01_->Set(j, i, j);
		}

	this->Sort(this->cf0_, this->clusters0_);
	this->Sort(this->cf1_, this->clusters1_);
	this->Sort(this->cf0f1_, this->clusters01_);

	//LOG4CXX_INFO(Sp::video_logger,clusters0_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,clusters1_->ToString());
	LOG4CXX_INFO(Sp::video_logger,clusters01_->ToString());

	//LOG4CXX_INFO(Sp::video_logger,cf0_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,cf1_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,cf0f1_->ToString());
}
}
