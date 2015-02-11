#include "TestFileUtils.h"

#include "FileUtils.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Utils;

CPPUNIT_TEST_SUITE_REGISTRATION (TestFileUtils);

void TestFileUtils::setUp(){
	mkdir("tmpfiles",0700);
}

void TestFileUtils::tearDown(){
	rmdir("tmpfiles");
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
	File::Write("tmpfiles/dummy.txt", "Hello World!",0660);
	symlink("dummy.txt", "tmpfiles/dummy.link.txt");
	CPPUNIT_ASSERT( File::FileExists("tmpfiles/dummy.txt") );
	CPPUNIT_ASSERT( File::FileExists("tmpfiles/dummy.link.txt") );
	CPPUNIT_ASSERT( File::LinkExists("tmpfiles/dummy.link.txt") == false );
	CPPUNIT_ASSERT( File::LinkExists("tmpfiles/dummy.txt") == false );
	CPPUNIT_ASSERT( File::LinkExists("tmpfiles") == false );
	CPPUNIT_ASSERT( File::LinkExists("nonsens") == false );
}

void
TestFileUtils::testFileRead ()
{
	std::vector<unsigned char> test={1,2,3,4,5,6,7,8,9,10};
	std::vector<unsigned char> verify;

	CPPUNIT_ASSERT_NO_THROW( File::WriteVector<std::vector<unsigned char>>("tmpfiles/test-1.bin", test, 0666) );
	CPPUNIT_ASSERT_NO_THROW( File::ReadVector<std::vector<unsigned char>>("tmpfiles/test-1.bin", verify) );
	CPPUNIT_ASSERT_EQUAL( test.size(), verify.size() );
	CPPUNIT_ASSERT( test == verify );

}

void TestFileUtils::testFileWrite()
{
	CPPUNIT_ASSERT_THROW( File::Write("tmpfiless/test-1.txt","Hello World!",0666), ErrnoException);
	CPPUNIT_ASSERT_NO_THROW( File::Write("tmpfiles/test-1.txt","Hello World!",0666) );
	CPPUNIT_ASSERT( File::FileExists("tmpfiles/test-1.txt") );
	unlink("tmpfiles/test-1.txt");
}
