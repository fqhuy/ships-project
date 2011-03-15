/*
 * model_test.hpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */

#ifndef MODEL_TEST_HPP_
#define MODEL_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <ships/core/core.hpp>

class HostMemoryModelTest: public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( HostMemoryModelTest );
		CPPUNIT_TEST( testCreateArray );
		//CPPUNIT_TEST( testConfig_stdstring );
	CPPUNIT_TEST_SUITE_END();

private:
	//Sp::HostMemoryModel model_;
	Sp::HostMemoryModel<int>* memory_;
	static const int SIZE = 32;
public:
	HostMemoryModelTest(){

	}

	virtual ~HostMemoryModelTest(){

	}
	void setUp() {
		memory_ = new Sp::HostMemoryModel<int>(2, 2);
	}

	void tearDown() {
		delete memory_;
	}

	void testCreateArray() {
		//LOG4CXX_INFO (Sp::core_logger, "\n----------testCreateArray----------\n");
		int* arr = memory_->CreateArray(SIZE);
		//char* carr = (char*) arr;
		for (int i = 0; i < SIZE; i++) {
			arr[i] = SIZE - i;
		}
		//for (int i = 0; i < SIZE; i++)
		//	LOG4CXX_INFO(Sp::core_logger, arr[i]);

		//delete []arr;
	}

};
//CPPUNIT_TEST_SUITE_REGISTRATION(HostMemoryModelTest);
#endif /* MODEL_TEST_HPP_ */
