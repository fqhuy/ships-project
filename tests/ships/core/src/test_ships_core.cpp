#include <cppunit/ui/text/TestRunner.h>
#include <ships/core/core.hpp>
#include "pixel_test.hpp"

int main(int argc, char **argv){
	  CppUnit::TextUi::TestRunner runner;
	  runner.addTest( PixelTest::suite() );
	  //runner.addTest( ComplexNumberTest::suite() );
	  runner.run();
	  return 0;
}
