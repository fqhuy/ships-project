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
	    Sp::Matrix<float,float>* matrix = NULL;
	    reader_.ReadAsFloatMatrix("data/frame0a",matrix);
	    
	    if(matrix==NULL)
	    	LOG4CXX_ERROR(Sp::io_logger, "plugin_octave_text_matrix_test.hpp: error in reading matrix.");

	    for(int i=0;i<matrix->GetHeight();i++)
	     // for(int j=0;j<matrix->GetWidth();j++)
	    	  LOG4CXX_INFO(Sp::io_logger, matrix->Get(i,0) << " : " << matrix->Get(i,1));

	    //delete matrix;
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(PluginOctaveTextMatrixTest);
#endif /* PLUGIN_OCTAVE_TEXT_MATRIX_TEST_HPP_ */
