#include "TestProcess.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Process.h"
#include "FileUtils.h"

using namespace  Utils;

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

	logg << Logger::Debug << "Testprocess start" <<lend;

	tie(status, reply) = Utils::Process::Exec("echo -n 'Hello'");

	CPPUNIT_ASSERT( status );
	CPPUNIT_ASSERT_EQUAL(string("Hello"), reply);

	tie(status, reply) = Utils::Process::Exec("no command  2> /dev/null");
	CPPUNIT_ASSERT( !status );

	CPPUNIT_ASSERT_NO_THROW(Utils::Process::Spawn( "/bin/ls" ) );

	File::Delete("/tmp/echotest");
	CPPUNIT_ASSERT_NO_THROW(Utils::Process::Spawn( "/usr/bin/touch",{"/tmp/echotest"} ) );
	sleep(1);
	CPPUNIT_ASSERT(File::FileExists("/tmp/echotest"));

	logg << Logger::Debug << "TestProcess complete" <<lend;
}
