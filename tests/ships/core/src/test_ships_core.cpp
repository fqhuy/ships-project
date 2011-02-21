#include <cppunit/ui/text/TestRunner.h>
#include <ships/core/core.hpp>
#include "matrix_test.hpp"
//#include "pixel_test.hpp"
//#include "config_test.hpp"
//#include "memory_model_test.hpp"

int main(int argc, char **argv) {
	Sp::ShipsConfig::Instance().Config();
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(MatrixTest::suite());
	//runner.addTest(PixelTest::suite());
	//runner.addTest(HostMemoryModelTest::suite());
	//runner.addTest( ConfigTest::suite() );
	//runner.addTest( ComplexNumberTest::suite() );
	runner.run();
	return 0;
}
