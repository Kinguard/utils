#include "TestProcess.h"

#include "Process.h"

CPPUNIT_TEST_SUITE_REGISTRATION ( TestProcess );

void TestProcess::setUp()
{
}

void TestProcess::tearDown()
{
}

void TestProcess::Test()
{

	bool status;
	string reply;

	tie(status, reply) = Utils::Process::Exec("echo -n 'Hello'");

	CPPUNIT_ASSERT( status );
	CPPUNIT_ASSERT_EQUAL(string("Hello"), reply);

	tie(status, reply) = Utils::Process::Exec("no command  2> /dev/null");
	CPPUNIT_ASSERT( !status );

}
