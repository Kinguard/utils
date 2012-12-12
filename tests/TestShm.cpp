#include "TestShm.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestSHM);

#include "Shm.h"
#include "FileUtils.h"

using namespace Utils;

void TestSHM::setUp(){
}

void TestSHM::tearDown(){
}

void TestSHM::test()
{

	CPPUNIT_ASSERT_THROW( Shm s("/none/existant", 4096), std::runtime_error );

	// FIle should be created
	{
		CPPUNIT_ASSERT_NO_THROW( Shm s("/tmp/testshm", 4096) );
		File::Delete("/tmp/testshm");
	}

	File::Write("/tmp/shmtest", "", 0644);
	CPPUNIT_ASSERT_NO_THROW( Shm s("/tmp/shmtest", 4096) );

	Shm s("/tmp/shmtest", 4096);

	char* data = static_cast< char* > ( s.Value() );

	sprintf(data, "Hello World!");

	Shm s2("/tmp/shmtest", 4096);
	char* d2 = static_cast< char* > ( s2.Value() );

	CPPUNIT_ASSERT( strcmp( data, d2 ) == 0 );

	s.Remove();
	s2.Remove();

	File::Delete("/tmp/shmtest");

}
