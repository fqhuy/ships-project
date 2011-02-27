/*
 * matrix_test.cpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */
#include "matrix_test.hpp"
//CPPUNIT_TEST_SUITE_REGISTRATION(DeviceMatrixTest);

DeviceMatrixTest::DeviceMatrixTest() {

	//Sp::MemoryModel<float>* mm = new Sp::MemoryModel<float>(1, true, true, 2,
	//		Sp::READ_WRITE);
	//uint32_t size = 7;
	//Sp::SampleModel<float, float>* sm = new Sp::PixelInterleavedSampleModel<
	//		float>(1, 1, &size);
	//vector_ = new Sp::DeviceMatrix<float, float>(size, mm, sm);

}

DeviceMatrixTest::~DeviceMatrixTest() {
}

void DeviceMatrixTest::setUp() {
	LOG4CXX_INFO(Sp::core_logger, "matrix_test: creating device matrix...");
	width_ = 4;
	height_ = 4;
	matrix_ = new Sp::DeviceMatrix<float, float>(width_ , height_);
}

void DeviceMatrixTest::tearDown() {

}

void DeviceMatrixTest::testSet() {

}

void DeviceMatrixTest::testGet() {

}

