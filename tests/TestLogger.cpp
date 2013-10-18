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
		lg << Logger::Emerg<< "Testing"<<end;

		std::string result="TestLogger, Testing\n";
		CPPUNIT_ASSERT_EQUAL(result,oss.str() );
#if 0
		lg.SetLevel(Logger::Emerg);
		lg << Logger::Emerg << "Emerg"<<end;
		lg << Logger::Alert << "Alert"<<end;
		lg << Logger::Crit << "Crit"<<end;
		lg << Logger::Error << "Error"<<end;
		lg << Logger::Warning << "Warning"<<end;
		lg << Logger::Notice << "Notice"<<end;
		lg << Logger::Info << "Info"<<end;
		lg << Logger::Debug << "Debug"<<end;

		lg << Logger::Debug << "Hello!"<<12<<end;

		log.SetLevel(Logger::Debug);
		log << "Hello from testworld"<<end;
		log.SetLogName("TestGlobalLogger");
		log << "Hello with name"<<end;
		log.SetTimeStamping(true);
		log << "Hello with timestamp"<<end;
		log.SetOutputter([](const std::string& msg){std::cerr << "CERR["<< msg<<"]";} );
		log << "Now from cerr"<<end;

		std::cout << "["<< oss.str()<<"]"<<std::endl;
#endif

	}
}
