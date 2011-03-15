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
	this->N_->Init(true);

	this->tau_->Init(this->tau0_);

	try {
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
	} catch (std::exception& e) {
		LOG4CXX_ERROR(Sp::video_logger, "An error occured: "<< e.what());
	}
	return this->result_;
}

}
