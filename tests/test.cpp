#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <syslog.h>
#include <string>
#include <map>

#include "Logger.h"

using namespace Utils;
using namespace std;


// These are the same I.e. Logger::Debug == LOG_DEBUG
// But to make sure this wont change we use a lookup map
static map<enum Logger::LogLevel, int> loggertosys =
{
	{ Logger::Debug,	LOG_DEBUG},
	{ Logger::Info,		LOG_INFO},
	{ Logger::Notice,	LOG_NOTICE},
	{ Logger::Warning,	LOG_WARNING},
	{ Logger::Error,	LOG_ERR},
	{ Logger::Crit,		LOG_CRIT},
	{ Logger::Alert,	LOG_ALERT},
	{ Logger::Emerg,	LOG_EMERG}
};


static int toSyslogLevel(enum Logger::LogLevel level)
{
	if( loggertosys.find(level) != loggertosys.end() )
	{
		return loggertosys[level];
	}
	else
	{
		return LOG_DEBUG;
	}

}


int main(int argc, char** argv){
	(void)argc;
	(void)argv;

	// Setup logging
	openlog( "libutils-test", LOG_PERROR, LOG_DAEMON);
	logg.SetOutputter( [](const string& msg){ syslog(toSyslogLevel(logg.getLevel()), "%s",msg.c_str());});
	logg.SetLogName("");
	logg.SetLevel(Logger::Error);

	logg << Logger::Debug << "Start testing" << lend;

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

	logg << Logger::Debug << "Test completed" << lend;

	// Run the tests.
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}
