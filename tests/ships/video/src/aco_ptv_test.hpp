/*
 * aco_ptv_test.hpp
 *
 *  Created on: Mar 3, 2011
 *      Author: fqhuy
 */

#ifndef ACO_PTV_TEST_HPP_
#define ACO_PTV_TEST_HPP_

class ACOPTVEstimatorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( ACOPTVEstimatorTest);

	CPPUNIT_TEST( testEstimate);
	//CPPUNIT_TEST( testRelaxationLength);
	//CPPUNIT_TEST( testDistances );
	//CPPUNIT_TEST( testSort );

	CPPUNIT_TEST_SUITE_END();

private:
	static const int SIZE = 16;
	static const int DIM = 2;
	Sp::Matrix<float, float>* frame0_;
	Sp::Matrix<float, float>* frame1_;
	Sp::Matrix<float, float>* result_;
	Sp::Matrix<int, int>* iresult_;
	Sp::ACOPTVEstimator* est_;

	float alpha;
	float beta;
	float rho;
	float tau0;
	int cluster_size;
	int num_particles;
	int num_ants;
	int num_loops;
public:
	ACOPTVEstimatorTest() :
		frame0_(NULL), frame1_(NULL), result_(NULL), iresult_(NULL), est_(NULL) {
		alpha = 1.0f;
		beta = 5.0f;
		num_loops = 2; //5;
		num_particles = 16; //1024, 2048, 3072, 4096
		num_ants = 8;
		rho = 0.5f;
		cluster_size = 2; //4
		tau0 = 5.0f;
	}
	virtual ~ACOPTVEstimatorTest() {
		if (frame0_)
			delete frame0_;
		if (frame1_)
			delete frame1_;
		if (result_)
			delete result_;
		if (iresult_)
			delete iresult_;
		if(est_)
			delete est_;
	}
	void setUp() {
		Sp::OctaveTextMatrixReader reader_;
		frame0_ = new Sp::HostMatrix<float, float>(DIM, SIZE);
		frame1_ = new Sp::HostMatrix<float, float>(DIM, SIZE);
		reader_.ReadAsFloatMatrix(
				"/home/fqhuy/Documents/Projects/ships-project-build/bin/data/frame0a.vec",
				frame0_);

		reader_.ReadAsFloatMatrix(
				"/home/fqhuy/Documents/Projects/ships-project-build/bin/data/frame1a.vec",
				frame1_);

		est_ = new Sp::ACOPTVEstimator(alpha, beta, rho, tau0, cluster_size,
				num_particles, num_ants, num_loops);
	}

	void tearDown() {

	}

	void testDistances() {


		result_ = est_->Distances(frame0_, frame0_);
		LOG4CXX_INFO(Sp::video_logger, frame0_->ToString());
		LOG4CXX_INFO(Sp::video_logger, frame1_->ToString());
		LOG4CXX_INFO(Sp::video_logger, result_->ToString());
	}

	void testSort() {
		//Sp::ACOPTVEstimator est;
		Sp::HostMatrix<int, int> indices(SIZE, SIZE);
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				indices.Set(j, i, j);

		result_ = est_->Distances(frame0_, frame1_);
		est_->Sort(result_, &indices);

		LOG4CXX_INFO(Sp::video_logger, result_->ToString());
		LOG4CXX_INFO(Sp::video_logger, indices.ToString());
	}
	void testEstimate() {
		//Sp::ACOPTVEstimator est(alpha, beta, rho, tau0, cluster_size,
		//		num_particles, num_ants, num_loops);
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		iresult_ = est_->Estimate();
		//LOG4CXX_INFO(Sp::video_logger,"estimating the motion vectors..");
		CPPUNIT_ASSERT(iresult_ != NULL);
		if(iresult_)
			LOG4CXX_INFO(Sp::video_logger, iresult_->ToString());
	}
	void testRelaxationLength() {
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		est_->Estimate();

		float length = est_->RelaxationLength(0,0);
		LOG4CXX_INFO(Sp::video_logger, length);
	}

	void testCluster() {

	}

	void testNext() {

	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( ACOPTVEstimatorTest);

#endif /* ACO_PTV_TEST_HPP_ */
