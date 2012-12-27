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
					[&oss](const std::string& s){ oss << s; }
				);

		lg << "Hello!"<<12<<end;

		std::cout << "["<< oss.str()<<"]"<<std::endl;
	}
}
