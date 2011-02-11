/*
 * pixel_test.hpp
 *
 *  Created on: Jan 30, 2011
 *      Author: fqhuy
 */

#ifndef PIXEL_TEST_HPP_
#define PIXEL_TEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class PixelTest: public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( PixelTest );
	CPPUNIT_TEST( testEquality );
	CPPUNIT_TEST( testAddition );
	CPPUNIT_TEST_SUITE_END();

	private:
		//Sp::HostPixel<char> p1;
		//Sp::HostPixel<char> p2;
		//Sp::Image<char> i1;
		Sp::ImageMetaData im1;
	public:
		void setUp() {
			//p2 = Sp::HostPixel<char>(123,213,0,0);
			//i1 = Sp::Image<char>();
			im1 = Sp::ImageMetaData();
		}

		void tearDown() {

		}

		void testEquality() {

		}

		void testAddition() {

		}
};

#endif /* PIXEL_TEST_HPP_ */
