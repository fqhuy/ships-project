/*
 * config_test.hpp
 *
 *  Created on: Feb 16, 2011
 *      Author: fqhuy
 */

#ifndef CONFIG_TEST_HPP_
#define CONFIG_TEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class ConfigTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( ConfigTest );
	CPPUNIT_TEST( testConfig );
	CPPUNIT_TEST( testConfig_stdstring );
	CPPUNIT_TEST_SUITE_END();

	private:

	public:
		void setUp() {

		}

		void tearDown() {

		}

		void testConfig() {
			//Sp::ShipsConfig::Instance().Config();
		}

		void testConfig_stdstring() {

		}
};

#endif /* CONFIG_TEST_HPP_ */
