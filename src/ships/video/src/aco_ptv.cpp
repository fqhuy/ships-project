#include <ships/video/video.hpp>

namespace Sp {

Matrix<int, int>* ACOPTVEstimator::Estimate() {
	/*-----------------------------Check some initial conditions-------------------------------------*/
	if (this->frames_.size() != 2)
		return NULL;
	else {
		this->f0_ = this->frames_[0];
		this->f1_ = this->frames_[1];
	}
	if (this->f0_ == NULL || this->f1_ == NULL)
		return NULL;

	if (this->f0_->GetHeight() != this->f1_->GetHeight())
		return NULL;
	/*-----------------------------Initialize matrices------------------------------------------------*/
	//this->tau_ = new HostMatrix<float, float> (this->num_particles_,
	//		this->num_particles_);
	//this->tau_->Init(this->tau0_);

	this->N_ = new HostVector<bool, bool> (this->num_particles_);
	this->N_->Init(true);
	/*-----------------------------Measure distances between particles--------------------------------*/
	this->cf0_ = this->Distances(this->f0_, this->f0_);
	if (!this->cf0_)
		return NULL;

	this->cf1_ = this->Distances(this->f1_, this->f1_);
	if (!this->cf1_)
		return NULL;

	this->cf0f1_ = this->Distances(this->f0_, this->f1_);
	if (!this->cf0f1_)
		return NULL;
	/*-----------------------------Cluster particles in a frame into groups----------------------------*/
	this->Cluster();
	/*-----------------------------Iterate the finding process-----------------------------------------*/
	this->Loop();

	return this->result_;
	//return NULL;
}

float ACOPTVEstimator::RelaxationLength(const int& p0, const int& p1) {
	float length = 0;
	float dx1, dx2, dy1, dy2, rx1, rx2, ry1, ry2, min;
	int r1, r2, temp;

	dx1 = this->f0_->Get(p0, 0);
	dy1 = this->f0_->Get(p0, 1);
	dx2 = this->f1_->Get(p1, 0);
	dy2 = this->f1_->Get(p1, 1);

	for (int i = 0; i < this->cluster_size_; i++) {
		//location of central particles.


		//get the ID of (i+1)-th particle in the cluster 0.
		r1 = this->clusters0_->Get(p0, i + 1);
		rx1 = this->f0_->Get(r1, 0);
		ry1 = this->f0_->Get(r1, 1);

		min = INT_MAX;
		//find within second cluster the closest particle
		for (int k = 0; k < this->cluster_size_; k++) {
			temp = this->clusters1_->Get(p1, k + 1);
			if (this->cf0f1_->Get(r1, temp) < min) {
				min = this->cf0f1_->Get(r1, temp);
				r2 = temp;
			}
		}
		rx2 = this->f1_->Get(r2, 0);
		ry2 = this->f1_->Get(r2, 1);

		length += (dx2 + rx1 - dx1 - rx2) * (dx2 + rx1 - dx1 - rx2) + (dy2
				+ ry1 - dy1 - ry2) * (dy2 + ry1 - dy1 - ry2);
	}

	return length;
}

int ACOPTVEstimator::Next(int current_pos, float* probability) {
	float prob = 1, eta = 0, denominator = 1;
	int selected = -1, cup;
	//finding Eta

	for (int i = 0; i < this->num_particles_; i++) {
		if (N_->Get(i) == true) {
			//currently eta is calculated as reciprocal of the distance between two particles.
			eta = 1 / this->cf0f1_->Get(current_pos, i);
			// comment out bellow line to use relaxation length. usually, we use conventional length for the 1st iteration. ***
			//eta = 1 / this->RelaxationLength(current_pos, i);

			denominator += (pow(this->tau_->Get(current_pos, i), this->alpha_)
					* pow(eta, this->beta_));
		}
	}

	//loop through all unvisited particles in second frame to find one with highest probability.
	float max = 0, tauij;
	//this loop can be speed up using the ordered matrix clusters01_.
	for (int i = 0; i < this->cluster_size_/*this->num_particles_*/; i++) {
		cup = this->clusters01_->Get(current_pos, i);
		if (N_->Get(cup) == true) {
			eta = 1 / this->cf0f1_->Get(current_pos, cup);
			//temporarily
			tauij = this->tau_->Get(current_pos, cup);
			prob = (pow(tauij, this->alpha_) * pow(eta, this->beta_))
					/ denominator;
			if (prob > max) {
				max = prob;
				selected = cup;
			}
		}
	}
	if (max == 0) {
		LOG4CXX_WARN(Sp::video_logger, "Can not find any sufficient particle, consider changing the number of particles to look at..");
		return -1;
	}

	//if probability is not NULL, set the value to prob
	if (probability != NULL)
		(*probability) = max;
	return selected;
}

void ACOPTVEstimator::Loop() {
	Random rd;
	int i, j, k, next;
	float tau, delta_tau, l, Lk, bestLk = INT_MAX;
	//final result
	int *rf0, *rf1;

	int np = this->num_particles_;
	int na = this->num_ants_;
	int nl = this->num_loops_;

	for (i = 0; i < nl; i++) { //for nl number of loops.
		//Tk0[ant][particle id]
		int Tk0[na][np];
		int Tk1[na][np];

		for (j = 0; j < na; j++) { //for every ant.
			this->N_->Init(true);
			//init with index value
			for (k = 0; k < np; k++)
				Tk0[j][k] = k;

			//randomize the itinerary of the ants by randomly exchanging values of elements.
			int temp, a, b;
			for (k = 0; k < np / 2; k++) { //for every particle
				a = rd.Next<int> (0, np - 1);
				b = rd.Next<int> (0, np - 1);
				temp = Tk0[j][a];
				Tk0[j][a] = Tk0[j][b];
				Tk0[j][b] = temp;
			}

			for (k = 0; k < np; k++) { //for every particle
				next = this->Next(Tk0[j][k]);
				if (next == -1) {
					break;
				}
				//update the route
				Tk1[j][k] = next;
				//mark this particle as visited
				this->N_->Set(false, next);
			}
		}

		//Update Tau matrix for every ant.
		for (j = 0; j < na; j++) {
			//Find Lk
			for (k = 0; k < np; k++)
				Lk += this->cf0f1_->Get(Tk0[j][k], Tk1[j][k]);
			// comment out to use relaxation length.
			//Lk = this->SumRelaxationLengths(Tk0[j], Tk1[j]);

			if (Lk < bestLk) {
				bestLk = Lk;
				rf0 = Tk0[j];
				rf1 = Tk1[j];
			}
			//find delta tau
			delta_tau = 1 / Lk;
			//update pheromone on each portion
			for (k = 0; k < np; k++) {
				tau = this->tau_->Get(Tk0[j][k], Tk1[j][k]);
				this->tau_->Set((1 - this->rho_) * tau + delta_tau, Tk0[j][k],
						Tk1[j][k]);
			}
		}

		if (i == (nl - 1)) {
			this->result_ = new HostMatrix<int, int> (2, np);
			for (i = 0; i < np; i++) {
				this->result_->Set(rf0[i], i, 0);
				this->result_->Set(rf1[i], i, 1);
			}
		}
	}

}

void ACOPTVEstimator::Sort(Matrix<float, float>* matrix,
		Matrix<int, int>* indices) {
	float* row = NULL;
	int* irow = NULL;
	Matrix<float, float>* clone = matrix->Clone();

	for (int i = 0; i < matrix->GetHeight(); i++) {
		row = clone->GetArray().GetData2D()[i];
		irow = indices->GetArray().GetData2D()[i];
		this->QuickSort<float> (row, irow, 0, clone->GetWidth() - 1);
	}
}

template<class T> void ACOPTVEstimator::QuickSort(T numbers[], int indices[],
		int left, int right) {
	T pivot;
	int ipivot, l_hold, r_hold;

	l_hold = left;
	r_hold = right;
	pivot = numbers[left];
	ipivot = indices[left];
	while (left < right) {
		while ((numbers[right] >= pivot) && (left < right))
			right--;
		if (left != right) {
			numbers[left] = numbers[right];
			indices[left] = indices[right];
			left++;
		}
		while ((numbers[left] <= pivot) && (left < right))
			left++;
		if (left != right) {
			numbers[right] = numbers[left];
			indices[right] = indices[left];
			right--;
		}
	}
	numbers[left] = pivot;
	indices[left] = ipivot;

	pivot = left;
	left = l_hold;
	right = r_hold;
	if (left < pivot)
		QuickSort(numbers, indices, left, pivot - 1);
	if (right > pivot)
		QuickSort(numbers, indices, pivot + 1, right);
}

Matrix<float, float>* ACOPTVEstimator::Distances(Matrix<float, float>* f0,
		Matrix<float, float>* f1) {

	if (f0 == NULL || f1 == NULL)
		return NULL;

	if (f0->GetHeight() != f1->GetHeight() || f0->GetWidth() != f1->GetWidth())
		return NULL;

	const int w = f0->GetWidth();
	const int h = f1->GetHeight();
	const int half = h / 2 + 1;

	Matrix<float, float>* result = new HostMatrix<float, float> (h, h);
	result->Zeros();
	//int a, b;
	float temp;
	/*
	 for (int i = 0; i < h; i++) {
	 a = 0;
	 b = i;
	 for (int j = 0; j < half; j++) {
	 if (b < h) {
	 a++;
	 b++;
	 temp = this->Distance(f0->Get(0,a),f0->Get(1,a),f0->Get(0,b),f0->Get(1,b));
	 result->Set(temp, a, b);
	 result->Set(temp, b, a);
	 } else {
	 a = half;
	 b = h - i + a;
	 temp = this->Distance(f0->Get(0,a),f0->Get(1,a),f0->Get(0,b),f0->Get(1,b));
	 result->Set(temp, a, b);
	 result->Set(temp, b, a);
	 //dis1(i,j);
	 }
	 }
	 }
	 */
	float x1, x2, y1, y2;
	int end = 0;
	for (int i = 0; i < h; i++) {
		x1 = f0->Get(i, 0);
		y1 = f0->Get(i, 1);
		for (int j = end; j < h; j++) {
			x2 = f1->Get(j, 0);
			y2 = f1->Get(j, 1);
			//temp = this->Distance(f0->Get(0,i),f0->Get(1,i),f1->Get(0,j),f1->Get(1,j));
			temp = this->Distance(x1, y1, x2, y2);
			result->Set(temp, i, j);
			//result->Set(temp, j, i);
		}
		//end++;
	}

	return result;
}

void ACOPTVEstimator::Cluster() {
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
	//LOG4CXX_INFO(Sp::video_logger,clusters01_->ToString());

	//LOG4CXX_INFO(Sp::video_logger,cf0_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,cf1_->ToString());
	//LOG4CXX_INFO(Sp::video_logger,cf0f1_->ToString());
}

}
