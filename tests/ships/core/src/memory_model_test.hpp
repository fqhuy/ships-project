/*
 * model_test.hpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */

#ifndef MODEL_TEST_HPP_
#define MODEL_TEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class HostMemoryModelTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( HostMemoryModelTest );
	CPPUNIT_TEST( testCreateArray );
	//CPPUNIT_TEST( testConfig_stdstring );
	CPPUNIT_TEST_SUITE_END();

	private:
		//Sp::HostMemoryModel model_;
	public:
		void setUp() {

		}

		void tearDown() {

		}

		void testCreateArray() {
			/*LOG4CXX_INFO (Sp::core_logger, "----------testCreateArray----------\n");
			void* arr = model_.CreateArray(10);
			char* carr = (char*) arr;
			for (int i=0;i<10;i++){
				carr[i] = (char) (97 + i);
			}
			for (int i=0;i<10;i++)
				LOG4CXX_INFO(Sp::core_logger, carr[i]);
				*/
		}

};


#endif /* MODEL_TEST_HPP_ */
