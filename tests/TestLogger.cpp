#include "TestLogger.h"

#include "Logger.h"

#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION (TestLogger);

using namespace Utils;

class Writer: private ClassWriter
{
public:
	Writer()
	{

	}

	virtual ~Writer()
	{

	}
};

void TestLogger::setUp(){
}

void TestLogger::tearDown(){
}

void TestLogger::testWriter()
{
	CPPUNIT_ASSERT_NO_THROW( Writer w );
}

void TestLogger::test()
{
	{
		std::ostringstream oss;
		Logger lg(
					[&oss](const std::string& s){ oss << s; },
					"TestLogger",
					false
				);
		lg << Logger::Emerg<< "Testing"<<lend;

		std::string result="TestLogger, Testing\n";
		CPPUNIT_ASSERT_EQUAL(result,oss.str() );
#if 0
		lg.SetLevel(Logger::Emerg);
		lg << Logger::Emerg << "Emerg"<<lend;
		lg << Logger::Alert << "Alert"<<lend;
		lg << Logger::Crit << "Crit"<<lend;
		lg << Logger::Error << "Error"<<lend;
		lg << Logger::Warning << "Warning"<<lend;
		lg << Logger::Notice << "Notice"<<lend;
		lg << Logger::Info << "Info"<<lend;
		lg << Logger::Debug << "Debug"<<lend;

		lg << Logger::Debug << "Hello!"<<12<<lend;

		log.SetLevel(Logger::Debug);
		log << "Hello from testworld"<<lend;
		log.SetLogName("TestGlobalLogger");
		log << "Hello with name"<<lend;
		log.SetTimeStamping(true);
		log << "Hello with timestamp"<<lend;
		log.SetOutputter([](const std::string& msg){std::cerr << "CERR["<< msg<<"]";} );
		log << "Now from cerr"<<lend;

		std::cout << "["<< oss.str()<<"]"<<std::endl;
#endif

	}
}
