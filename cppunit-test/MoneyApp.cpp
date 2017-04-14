
#include "GccOutputter.h"
#include "MSVCOutputter.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>

int main()
{
  // Get the top level suite from the registry
  CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

  CPPUNIT_NS::TestResult controller;

  // Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );

  // Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );

  // Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;

  runner.run( controller );
  runner.addTest( suite );

  // Run the test.
  runner.run(controller);
  bool wasSucessful = result.wasSuccessful();

  // Print test in a compiler compatible format.
  #ifndef _WIN32
	  GccOutputter outputter( &result, std::cerr );
  #else
	  MSVCOutputter outputter( &result, std::cout );
  #endif
  outputter.write();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}

