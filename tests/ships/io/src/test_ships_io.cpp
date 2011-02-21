/*
 * test_ships_io.cpp
 *
 *  Created on: Jan 29, 2011
 *      Author: fqhuy
 */

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <ships/io/io.hpp>

#include "plugin_png_test.hpp"
#include "iostream_test.hpp"

int main(int argc, char **argv) {
	Sp::ShipsConfig::Instance().Config();
	CppUnit::TextUi::TestRunner runner;
	//runner.addTest( ExampleTestCase::suite() );
	runner.addTest(PluginPNGTest::suite());
	runner.addTest(FileInputStreamTest::suite());
	runner.run();
	return 0;
}
