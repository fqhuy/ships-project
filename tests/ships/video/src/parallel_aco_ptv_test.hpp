/*
 * parallel_aco_ptv_test.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef PARALLEL_ACO_PTV_TEST_HPP_
#define PARALLEL_ACO_PTV_TEST_HPP_

class ParallelACOPTVEstimatorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( ParallelACOPTVEstimatorTest);

	CPPUNIT_TEST( testEstimate);

	CPPUNIT_TEST_SUITE_END();

private:
	Sp::Matrix<float, float>* frame0_;
	Sp::Matrix<float, float>* frame1_;
	Sp::Matrix<float, float>* result_;
public:
	ParallelACOPTVEstimatorTest(): frame0_(NULL), frame1_(NULL), result_(NULL) {
	}
	virtual ~ParallelACOPTVEstimatorTest() {
	}
	void setUp() {
		Sp::OctaveTextMatrixReader reader_;
		frame0_ = new Sp::Matrix<float,float>(2,1024);
		frame1_ = new Sp::Matrix<float,float>(2,1024);
		reader_.ReadAsFloatMatrix(
				"/home/fqhuy/Documents/Projects/ships-project-build/bin/data/frame0a",
				frame0_);

		reader_.ReadAsFloatMatrix(
				"/home/fqhuy/Documents/Projects/ships-project-build/bin/data/frame1a",
				frame1_);
	}

	void tearDown() {
		if (frame0_)
			delete frame0_;
		if (frame1_)
			delete frame1_;
		if (result_)
			delete result_;
	}
	void testEstimate() {
		Sp::ParallelACOPTVEstimator est;
		est.AddFrame(frame0_);
		est.AddFrame(frame1_);
		//result_ = est.Estimate();
		//LOG4CXX_INFO(Sp::video_logger,"estimating the motion vectors..");
		CPPUNIT_ASSERT(est.Estimate()!=NULL);
	}
};

//CPPUNIT_TEST_SUITE_REGISTRATION( ParallelACOPTVEstimatorTest);
#endif /* PARALLEL_ACO_PTV_TEST_HPP_ */
