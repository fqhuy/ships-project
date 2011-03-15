/*
 * array_test.hpp
 *
 *  Created on: Feb 12, 2011
 *      Author: fqhuy
 */

#ifndef ARRAY_TEST_HPP_
#define ARRAY_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

class ArrayTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( ArrayTest );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST( testSet );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::Array<int> *array_;
		static const int WIDTH = 8;
		static const int HEIGHT = 8;
	public:
		ArrayTest(): array_(NULL){

		}
		void setUp() {
			Sp::MemoryModel<int>* mm = new Sp::MemoryModel<int>(2,1);
			cl::ImageFormat imf;
			imf.image_channel_order = CL_A;
			imf.image_channel_data_type = CL_UNSIGNED_INT32;
			mm->SetImageFormat(imf);
			array_ = new Sp::Array<int>(WIDTH, HEIGHT,mm);
		}

		void testSet(){
			for(int i=0;i<WIDTH;i++){
				for(int j=0;j<HEIGHT;j++){
					array_->Set(i+j,i,j);
				}
			}
			//LOG4CXX_INFO(Sp::core_logger, array_->ToString());
		}

		void tearDown() {
			if(array_)
				delete array_;
		}

		void testClone() {
			testSet();

			Sp::Array<int>* clone = array_->Clone();
			//LOG4CXX_INFO(Sp::core_logger, clone->ToString());
			if(clone)
				delete clone;
		}

		void testAddition() {

		}
};
//CPPUNIT_TEST_SUITE_REGISTRATION(ArrayTest);

class HostArrayTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( HostArrayTest );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST( testToString );
	CPPUNIT_TEST_SUITE_END();

	private:
		Sp::HostArray<int>* array_;
		static const int WIDTH = 8;
		static const int HEIGHT = 8;
	public:
		HostArrayTest(){
			array_ = NULL;
		}
		void setUp() {
			Sp::HostMemoryModel<int>* mm = new Sp::HostMemoryModel<int>(2,2);
			array_ = new Sp::HostArray<int>(WIDTH, HEIGHT,mm);
		}

		void tearDown() {
			if(array_)
				delete array_;
		}

		void testSet(){
			for(int i=0;i<WIDTH;i++){
				for(int j=0;j<HEIGHT;j++){
					array_->Set(i+j,i,j);
				}
			}
		}

		void testClone() {
			testSet();
			Sp::Array<int>* clone = array_->Clone();
			//LOG4CXX_INFO(Sp::core_logger, clone->ToString());
			for(int i=0;i<WIDTH;i++)
				for(int j=0;j<HEIGHT;j++)
					CPPUNIT_ASSERT_EQUAL(clone->Get(i,j),array_->Get(i,j));
			delete clone;
		}

		void testToString(){
			testSet();
			//LOG4CXX_INFO(Sp::core_logger, array_->ToString());
		}

		void testAddition() {

		}
};

//CPPUNIT_TEST_SUITE_REGISTRATION(HostArrayTest);

#endif /* ARRAY_TEST_HPP_ */
