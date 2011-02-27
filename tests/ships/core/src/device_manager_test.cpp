/*
 * device_manager_test.cpp
 *
 *  Created on: Feb 27, 2011
 *      Author: fqhuy
 */


#include "device_manager_test.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(DeviceManagerTest);

DeviceManagerTest::DeviceManagerTest() {
}

DeviceManagerTest::~DeviceManagerTest() {
}

void DeviceManagerTest::setUp() {

}

void DeviceManagerTest::tearDown() {

}

void DeviceManagerTest::testFindKernel() {
	cl::Kernel* kernel = Sp::DeviceManager::Instance().FindKernel(std::string("ships.core.vector.add"));
	CPPUNIT_ASSERT(kernel!=NULL);
	LOG4CXX_INFO(Sp::core_logger, "kernel ID is: " << (*kernel)());
}

