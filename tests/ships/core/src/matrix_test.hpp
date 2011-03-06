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
	CPPUNIT_TEST( testClone );
	//CPPUNIT_TEST( testSub );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Matrix<int, int>* matrix_;
		int width_, height_;
	public:
		void setUp() {
			//LOG4CXX_INFO(Sp::core_logger, "matrix_test: creating matrix...");
			width_ = 4;
			height_ = 4;

			matrix_ = new Sp::Matrix<int, int>(width_ , height_);

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
		void testSet(){
			for(int i=0;i<width_;i++)
				for(int j=0;j<height_;j++)
					matrix_->Set(i*j,i,j);
		}
		void testSub() {

		}

		void testClone(){
			testSet();
			Sp::Matrix<int,int>* clone = NULL;
			clone = matrix_->Clone();

			LOG4CXX_INFO(Sp::core_logger, clone->ToString());
			if(clone)
				delete clone;
		}
};
CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);
class HostMatrixTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( HostMatrixTest );
	//CPPUNIT_TEST( testSet );
	//CPPUNIT_TEST( testGet );
	//CPPUNIT_TEST( testToString );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Matrix<int, int>* matrix_;
		static const int WIDTH = 8;
		static const int HEIGHT = 8;
	public:
		void setUp() {
			matrix_ = new Sp::HostMatrix<int, int>(WIDTH , HEIGHT);
		}

		void tearDown() {
			delete matrix_;
		}
		void testSet(){
			//LOG4CXX_INFO(Sp::core_logger, "\ntestSet()\n");
			for(int i=0;i<WIDTH;i++)
				for(int j=0;j<HEIGHT;j++){
					matrix_->Set(i+j,i,j);
				}
		}
		void testGet(){
			testSet();
			for(int i=0;i<WIDTH;i++)
				for(int j=0;j<HEIGHT;j++){
					CPPUNIT_ASSERT(matrix_->Get(i,j)==i+j);
				}
		}
		void testAdd() {
			//int value = 10;
			//matrix_->Add(value);

			//for(int i=0;i<width_;i++)
			//	for(int j=0;j<height_;j++)
			//		LOG4CXX_INFO(Sp::core_logger, matrix_->Get(i,j));
		}

		void testToString(){
			testSet();
			LOG4CXX_INFO(Sp::core_logger, "\n" << matrix_->ToString());
		}

		void testClone(){
			testSet();
			Sp::Matrix<int,int>* clone = NULL;
			clone = matrix_->Clone();

			LOG4CXX_INFO(Sp::core_logger, clone->ToString());
			if(clone)
				delete clone;
		}

		void testSub() {

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(HostMatrixTest);

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
