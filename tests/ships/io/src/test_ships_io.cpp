/*
 * test_ships_io.cpp
 *
 *  Created on: Jan 29, 2011
 *      Author: fqhuy
 */

#include <cppunit/ui/text/TestRunner.h>
#include "plugin_png_test.hpp"

int main(int argc, char **argv){
	  CppUnit::TextUi::TestRunner runner;
	  //runner.addTest( ExampleTestCase::suite() );
	  runner.addTest( PluginPNGTest::suite() );
	  runner.run();
	  return 0;
}
