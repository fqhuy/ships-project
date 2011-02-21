/*
 * iostream_test.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */

#ifndef IOSTREAM_TEST_HPP_
#define IOSTREAM_TEST_HPP_


class FileInputStreamTest: public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( FileInputStreamTest );
CPPUNIT_TEST( testReadLine );
//CPPUNIT_TEST( testAddition );
CPPUNIT_TEST_SUITE_END();

private:
	Sp::FileInputStream* fis_;
public:
	void setUp() {
		fis_ = new Sp::FileInputStream("data/octave_matrix");
	}

	void tearDown() {
		delete fis_;
	}

	void testReadLine() {
		std::string s = fis_->ReadLine(1024);
		LOG4CXX_INFO(Sp::io_logger,s);
	}

};


#endif /* IOSTREAM_TEST_HPP_ */
