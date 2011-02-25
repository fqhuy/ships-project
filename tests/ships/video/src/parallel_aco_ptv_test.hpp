/*
 * parallel_aco_ptv_test.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef PARALLEL_ACO_PTV_TEST_HPP_
#define PARALLEL_ACO_PTV_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

class ParallelACOPTVEstimatorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( VectorTest );

	//CPPUNIT_TEST( testSet );

	CPPUNIT_TEST_SUITE_END();

private:

public:
	ParallelACOPTVEstimatorTest();
	virtual ~ParallelACOPTVEstimatorTest();
	void setUp() ;
	void tearDown() ;
};

#endif /* PARALLEL_ACO_PTV_TEST_HPP_ */
