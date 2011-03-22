/*
 * aco_ptv_test_simple.hpp
 *
 *  Created on: Mar 22, 2011
 *      Author: fqhuy
 */

#ifndef ACO_PTV_TEST_SIMPLE_HPP_
#define ACO_PTV_TEST_SIMPLE_HPP_

class ACOPTVEstimatorTestSimple: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( ACOPTVEstimatorTest);
	//CPPUNIT_TEST( testStandard);
	//CPPUNIT_TEST( testOrigin );
	//CPPUNIT_TEST( testEstimate );
	//CPPUNIT_TEST( testRelaxationLength);
	//CPPUNIT_TEST( testDistances );
	//CPPUNIT_TEST( testSort );
	//CPPUNIT_TEST( testCluster );
	CPPUNIT_TEST( testAll);

	CPPUNIT_TEST_SUITE_END();
private:
	static const int SIZE = 64;
public:
	ACOPTVEstimatorTestSimple(){

	}

	virtual ~ACOPTVEstimatorTestSimple(){

	}
	void setUp(){

	}
	void tearDown(){

	}

	void testAll(){
		//Sp::ACOPTVEstimator est(alpha,beta,rho,tau0,cluster_size,cluster_max,cluster01_max,num_particles,num_ants,num_loops);
		Sp::Matrix<int,int> result(SIZE,SIZE);

		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++){
				result.Set(SIZE - j, i, j);
			}
		}

		try{

		}catch(...){

		}
	}

};
//CPPUNIT_TEST_SUITE_REGISTRATION( ACOPTVEstimatorTestSimple);

#endif /* ACO_PTV_TEST_SIMPLE_HPP_ */
