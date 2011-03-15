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

		int width_ = 2;
		int height_ = 512;
		//LOG4CXX_INFO(Sp::core_logger, "matrix_test: creating matrix...");
		uint32_t dims[]  = {width_, height_};
		Sp::SampleModel<float,float>* sm = new Sp::PixelInterleavedSampleModel<float>(1,2,dims);
		Sp::MemoryModel<float>* mm = new Sp::MemoryModel<float>(1,true,true,2,Sp::READ);

		matrix = new Sp::Matrix<float,float>(width_,height_,mm,sm);

	    reader_.ReadAsFloatMatrix("data/frame0a",matrix);
	    
	    if(matrix==NULL)
	    	LOG4CXX_ERROR(Sp::io_logger, "plugin_octave_text_matrix_test.hpp: error in reading matrix.");

	    LOG4CXX_INFO(Sp::io_logger, matrix->ToString());

	    delete matrix;
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(PluginOctaveTextMatrixTest);
#endif /* PLUGIN_OCTAVE_TEXT_MATRIX_TEST_HPP_ */
