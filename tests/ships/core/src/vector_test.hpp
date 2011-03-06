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

class HostVectorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( HostVectorTest );
	//CPPUNIT_TEST( testSet );
	CPPUNIT_TEST( testGet );
	CPPUNIT_TEST_SUITE_END();

private:
	Sp::HostVector<bool, bool>* vector_;
public:
	HostVectorTest(){

	}
	virtual ~HostVectorTest(){

	}
	void setUp() {
		vector_ = new Sp::HostVector<bool,bool>(8);
	}
	void tearDown() {

	}
	void testSet() {
		for(int i=0;i<8;i++)
			vector_->Set((bool)(i % 2),i);
	}
	void testGet() {
		testSet();
		for(int i=0;i<8;i++){
			if(vector_->Get(i)){
				LOG4CXX_INFO(Sp::core_logger, "true");
			}
			else{
				LOG4CXX_INFO(Sp::core_logger, "false");
			}
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(HostVectorTest);
#endif /* VECTOR_TEST_HPP_ */
