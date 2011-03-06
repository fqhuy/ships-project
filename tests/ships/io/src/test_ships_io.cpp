/*
 * test_ships_io.cpp
 *
 *  Created on: Jan 29, 2011
 *      Author: fqhuy
 */
/*
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <ships/io/io.hpp>

#include "plugin_png_test.hpp"
#include "iostream_test.hpp"
#include "plugin_octave_text_matrix_test.hpp"

int main(int argc, char **argv) {
	Sp::ShipsConfig::Instance().Config();
	CppUnit::TextUi::TestRunner runner;
	//runner.addTest( ExampleTestCase::suite() );
	//runner.addTest(PluginPNGTest::suite());
	//runner.addTest(FileInputStreamTest::suite());
	runner.addTest(PluginOctaveTextMatrixTest::suite());
	runner.run();
	return 0;
}
*/
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ships/io/io.hpp>
#include "plugin_octave_text_matrix_test.hpp"

int main(int argc, char **argv) {
	Sp::ShipsConfig::Instance().Config();
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that colllects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter(&result, CPPUNIT_NS::stdCOut());
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}
