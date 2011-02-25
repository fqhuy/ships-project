/*
 * matrix_test.hpp
 *
 *  Created on: Feb 19, 2011
 *      Author: fqhuy
 */

#ifndef MATRIX_TEST_HPP_
#define MATRIX_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <ships/core/core.hpp>

class MatrixTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( MatrixTest );
	CPPUNIT_TEST( testAdd );
	CPPUNIT_TEST( testSub );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Matrix<int, int>* matrix_;
		int width_, height_;
	public:
		void setUp() {
			LOG4CXX_INFO(Sp::core_logger, "matrix_test: creating matrix...");
			width_ = 4;
			height_ = 4;

			matrix_ = new Sp::Matrix<int, int>(width_ , height_);
			for(int i=0;i<width_;i++)
				for(int j=0;j<height_;j++)
					matrix_->Set(i*j,i,j);
		}

		void tearDown() {
			//delete matrix_;
		}

		void testAdd() {
			//int value = 10;
			//matrix_->Add(value);

			//for(int i=0;i<width_;i++)
			//	for(int j=0;j<height_;j++)
			//		LOG4CXX_INFO(Sp::core_logger, matrix_->Get(i,j));
		}

		void testSub() {

		}

};

class DeviceMatrixTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( DeviceMatrixTest );
	CPPUNIT_TEST( testSet );
	CPPUNIT_TEST( testGet );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Matrix<float, float>* matrix_;
		int width_, height_;
	public:
		DeviceMatrixTest();
		virtual ~DeviceMatrixTest();
		void setUp() ;
		void tearDown() ;
		void testSet() ;
		void testGet() ;

};
#endif /* MATRIX_TEST_HPP_ */
