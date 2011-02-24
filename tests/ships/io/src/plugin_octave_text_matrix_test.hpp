#ifndef PLUGIN_OCTAVE_TEXT_MATRIX_TEST_HPP_
#define PLUGIN_OCTAVE_TEXT_MATRIX_TEST_HPP_


class PluginOctaveTextMatrixTest: public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( PluginOctaveTextMatrixTest );
CPPUNIT_TEST( testReadAsDoubleMatrix );
//CPPUNIT_TEST( testAddition );
CPPUNIT_TEST_SUITE_END();

private:
	Sp::OctaveTextMatrixReader reader_;
public:
	void setUp() {
	    //reader_.SetInputStream(new FileInputStream("data/octave_matrix");
	}

	void tearDown() {

	}

	void testReadAsDoubleMatrix() {
	    Sp::Matrix<double,double>* matrix = NULL;
	    reader_.ReadAsDoubleMatrix("data/octave_matrix",matrix);
	    
	    if(matrix==NULL)
	    	LOG4CXX_ERROR(Sp::io_logger, "plugin_octave_text_matrix_test.hpp: error in reading matrix.");

	    for(int i=0;i<matrix->GetHeight();i++)
	      for(int j=0;j<matrix->GetWidth();j++)
	    	  LOG4CXX_INFO(Sp::io_logger, matrix->Get(i,j));

	    //delete matrix;
	}

};


#endif /* PLUGIN_OCTAVE_TEXT_MATRIX_TEST_HPP_ */
