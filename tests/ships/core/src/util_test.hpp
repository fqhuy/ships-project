/*
 * util_test.hpp
 *
 *  Created on: Feb 28, 2011
 *      Author: fqhuy
 */

#ifndef UTIL_TEST_HPP_
#define UTIL_TEST_HPP_


class UtilTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( UtilTest );
	CPPUNIT_TEST( testDeltaT );
	CPPUNIT_TEST_SUITE_END();

private:
	Sp::Vector<float, float>* vector_;
public:
	UtilTest(){}
	virtual ~UtilTest(){}
	void setUp() {}
	void tearDown(){}

	void testDeltaT(){
		Sp::Timer::deltaT(0);
		int* a = new int[10000];
		for(int i=0;i<10000;i++)
			a[i] = 10000 - i;
		double e = Sp::Timer::deltaT(0);

		LOG4CXX_INFO(Sp::core_logger, "elapsed time is: "<<e);
	}

};
CPPUNIT_TEST_SUITE_REGISTRATION(UtilTest);
#endif /* UTIL_TEST_HPP_ */
