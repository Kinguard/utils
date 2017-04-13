#include "TestFileUtils.h"

#include "FileUtils.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Utils;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (TestFileUtils);

#define TESTDIR "/tmp/testfiles/"

void TestFileUtils::setUp(){
	mkdir(TESTDIR,0700);
}

void TestFileUtils::tearDown(){
	// Remove all files in testdir
	list<string> files = File::Glob(TESTDIR "*");
    for( const string& file: files)
    {
        if (File::DirExists(file))
        {
            rmdir(file.c_str());
        }
        else
        {
            unlink(file.c_str());
        }
    }

	rmdir(TESTDIR);
}

void TestFileUtils::testFileExists()
{
	CPPUNIT_ASSERT(File::FileExists("/proc/self/cmdline") );
	CPPUNIT_ASSERT(File::FileExists("nonsens") == false );
	CPPUNIT_ASSERT(File::FileExists("/proc") == false );
}

void TestFileUtils::testDirExists()
{
	CPPUNIT_ASSERT( File::DirExists("/proc") );
	CPPUNIT_ASSERT( File::DirExists("/proc/self/cmdline") == false );
	CPPUNIT_ASSERT( File::DirExists("nonsens") == false );
}

void TestFileUtils::testLinkExists()
{
	File::Write(TESTDIR "dummy.txt", "Hello World!",0660);
	symlink("dummy.txt", TESTDIR "dummy.link.txt");

	CPPUNIT_ASSERT( File::FileExists( TESTDIR "dummy.txt") );
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "dummy.link.txt") == false );
	CPPUNIT_ASSERT( File::LinkExists( TESTDIR "dummy.link.txt") );
	CPPUNIT_ASSERT( File::LinkExists( TESTDIR "dummy.txt") == false );
	CPPUNIT_ASSERT( File::LinkExists("tmpfiles") == false );
	CPPUNIT_ASSERT( File::LinkExists("nonsens") == false );
}

void TestFileUtils::testFileRead ()
{
	std::vector<unsigned char> test={1,2,3,4,5,6,7,8,9,10};
	std::vector<unsigned char> verify;

	CPPUNIT_ASSERT_NO_THROW( File::WriteVector<std::vector<unsigned char>>( TESTDIR "test-1.bin", test, 0666) );
	CPPUNIT_ASSERT_NO_THROW( File::ReadVector<std::vector<unsigned char>>( TESTDIR "test-1.bin", verify) );
	CPPUNIT_ASSERT_EQUAL( test.size(), verify.size() );
	CPPUNIT_ASSERT( test == verify );

}

void TestFileUtils::testFileWrite()
{
	CPPUNIT_ASSERT_THROW( File::Write("tmpfiless/test-1.txt","Hello World!",0666), ErrnoException);
	CPPUNIT_ASSERT_NO_THROW( File::Write( TESTDIR "test-1.txt","Hello World!",0666) );
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "test-1.txt") );
	unlink( TESTDIR "test-1.txt");
}

void TestFileUtils::testRealPath()
{
	File::Write( TESTDIR "rctest.txt", "Hello World!",0660);
	symlink("rctest.txt", TESTDIR "rctest.link.txt");

	CPPUNIT_ASSERT_EQUAL(  std::string(TESTDIR "rctest.txt"),  File::RealPath( TESTDIR "rctest.link.txt"));
	CPPUNIT_ASSERT_EQUAL( File::RealPath( TESTDIR "rctest.txt"),  File::RealPath( TESTDIR "rctest.link.txt"));
	CPPUNIT_ASSERT_EQUAL( File::RealPath( TESTDIR "rctest.txt"),  File::RealPath( TESTDIR "rctest.txt"));
	CPPUNIT_ASSERT_EQUAL( std::string( TESTDIR "rctest.txt"),  File::RealPath( TESTDIR "rctest.link.txt"));
	CPPUNIT_ASSERT_THROW( File::RealPath( TESTDIR "rctest.nonsens.txt"), ErrnoException );
}

void TestFileUtils::testMoveFile()
{
    File::Write(TESTDIR "dummy1.txt", "Hello World!",0660);
    File::Write(TESTDIR "dummy2.txt", "Hello World!",0660);

    File::Move( TESTDIR "dummy1.txt", TESTDIR "newdummy.txt");
    CPPUNIT_ASSERT( File::FileExists( TESTDIR "newdummy.txt") );
    CPPUNIT_ASSERT( File::FileExists( TESTDIR "dummy.txt") == false );

    CPPUNIT_ASSERT_THROW( File::Move( TESTDIR "newdummy.txt", TESTDIR "dummy2.txt"), ErrnoException);
}

void TestFileUtils::testMoveDir()
{
    mkdir(TESTDIR "/tmp",0700);

    File::Move( TESTDIR "/tmp", TESTDIR "/newdir");
    CPPUNIT_ASSERT( File::DirExists( TESTDIR "/newdir") );
    CPPUNIT_ASSERT( File::DirExists( TESTDIR "/tmp") == false );

    CPPUNIT_ASSERT_THROW( File::Move( TESTDIR "/missing", TESTDIR "/dontcare"), std::runtime_error);
    CPPUNIT_ASSERT_THROW( File::Move( TESTDIR "/newdir", "/root"), ErrnoException);
}
