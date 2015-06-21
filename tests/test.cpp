#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main(int argc, char** argv){
	// Get the top level suite from the registry
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	//CppUnit::TextTestRunner runner;
	runner.addTest( suite );

#if 0
	// Change the default outputter to a compiler error format outputter
	runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );
#else
	runner.setOutputter( new CppUnit::TextOutputter( &runner.result(), std::cerr ));
#endif

	// Run the tests.
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}
