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
		char name[256];
		char type[256];
		int rows, cols;

		s = fis_->ReadLine(1024);
		sscanf(s.c_str(),"# name: %s",name);

		s = fis_->ReadLine(1024);
		sscanf(s.c_str(),"# type: %s",type);

		s = fis_->ReadLine(1024);
		sscanf(s.c_str(),"# rows: %d",&rows);

		s = fis_->ReadLine(1024);
		sscanf(s.c_str(),"# columns: %d",&cols);

		float data[rows][cols];
		LOG4CXX_INFO(Sp::io_logger, "name: " << name << " type: " << type << " rows: " << rows << " cols: " << cols << '\n');
		/*
		for(int i=0;i<rows;i++){
			fis_->ReadLine(128,' ');
			for (int j=0;j<cols-1;j++){
				s = fis_->ReadLine(128,' ');
				sscanf(s.c_str(),"%f",&data[i][j]);
				LOG4CXX_INFO(Sp::io_logger, " " << data[i][j]);
			}
			s = fis_->ReadLine(128,'\n');
			sscanf(s.c_str(),"%f",&data[i][cols-1]);
			LOG4CXX_INFO(Sp::io_logger, " " << data[i][cols-1] << "\n");
		}
		*/


	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(FileInputStreamTest);
#endif /* IOSTREAM_TEST_HPP_ */
