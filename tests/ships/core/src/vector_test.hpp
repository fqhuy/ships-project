/*
 * vector_test.hpp
 *
 *  Created on: Feb 23, 2011
 *      Author: fqhuy
 */

#ifndef VECTOR_TEST_HPP_
#define VECTOR_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <ships/core/core.hpp>

class VectorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( VectorTest );

	CPPUNIT_TEST( testSet );

	CPPUNIT_TEST( testGet );

	CPPUNIT_TEST_SUITE_END();

private:
	Sp::Vector<float, float>* vector_;
public:
	VectorTest();
	virtual ~VectorTest();
	void setUp() ;
	void tearDown() ;
	void testSet() ;
	void testGet() ;
};

#endif /* VECTOR_TEST_HPP_ */
