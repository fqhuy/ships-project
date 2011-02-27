/*
 * device_manager_test.hpp
 *
 *  Created on: Feb 27, 2011
 *      Author: fqhuy
 */

#ifndef DEVICE_MANAGER_TEST_HPP_
#define DEVICE_MANAGER_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <ships/core/core.hpp>

class DeviceManagerTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( DeviceManagerTest );

	CPPUNIT_TEST( testFindKernel );

	CPPUNIT_TEST_SUITE_END();

private:
	//Sp::Vector<float, float>* vector_;
public:
	DeviceManagerTest();
	virtual ~DeviceManagerTest();
	void setUp() ;
	void tearDown() ;

	void testFindKernel();
};

#endif /* DEVICE_MANAGER_TEST_HPP_ */
