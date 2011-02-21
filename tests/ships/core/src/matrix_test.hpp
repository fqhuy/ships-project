/*
 * matrix_test.hpp
 *
 *  Created on: Feb 19, 2011
 *      Author: fqhuy
 */

#ifndef MATRIX_TEST_HPP_
#define MATRIX_TEST_HPP_


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class MatrixTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( MatrixTest );
	CPPUNIT_TEST( testAdd );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Matrix<int, int>* matrix_;
		int width_, height_;
	public:
		void setUp() {
			width_ = 300;
			height_ = 300;

			matrix_ = new Sp::Matrix<int, int>(width_ , height_);
			for(int i=0;i<width_;i++)
				for(int j=0;j<height_;j++)
					matrix_->Set(1,i,j);
		}

		void tearDown() {
			delete matrix_;
		}

		void testAdd() {
			int value = 10;
			matrix_->Add(value);

			for(int i=0;i<width_;i++)
				for(int j=0;j<height_;j++)
					LOG4CXX_INFO(Sp::core_logger, matrix_->Get(i,j));
		}

};

#endif /* MATRIX_TEST_HPP_ */
