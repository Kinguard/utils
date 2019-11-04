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
	//cout << "\nSetup" << endl;
	mkdir(TESTDIR,0700);
}


static void deldir(const string path)
{
	list<string> files = File::Glob(path + "/*");

	for( const string& file: files)
	{
		if (File::DirExists(file))
		{
			deldir(file);
		}
		else
		{
			unlink(file.c_str());
		}
	}

	rmdir(path.c_str() );
}


void TestFileUtils::tearDown(){
	//cout << "\nTeardown" << endl;

	deldir(TESTDIR);
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
	// Write string
	CPPUNIT_ASSERT_THROW( File::Write("tmpfiless/test-1.txt","Hello World!",0666), ErrnoException);
	CPPUNIT_ASSERT_NO_THROW( File::Write( TESTDIR "test-1.txt","Hello World!",0666) );
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "test-1.txt") );
	unlink( TESTDIR "test-1.txt");

	// Write list of strings
	list<string> data = {"hello\n", "cruel\n", "world\n"};
	list<string> empty, verify;

	CPPUNIT_ASSERT_NO_THROW(File::Write(TESTDIR "test-list.txt", data , 0666));
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "test-list.txt") );
	CPPUNIT_ASSERT_NO_THROW( verify = File::GetContent( TESTDIR "test-list.txt") );
	CPPUNIT_ASSERT_EQUAL( data.size(), verify.size());
	unlink( TESTDIR "test-list.txt");

	CPPUNIT_ASSERT_NO_THROW(File::Write(TESTDIR "test-list.txt", empty , 0666));
	CPPUNIT_ASSERT_NO_THROW( verify = File::GetContent( TESTDIR "test-list.txt") );
	CPPUNIT_ASSERT( verify.size() == 0);
	unlink( TESTDIR "test-list.txt");

	// Safe write string
	CPPUNIT_ASSERT_NO_THROW( File::SafeWrite(TESTDIR "safe.txt", "Hello world", 0666));
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "safe.txt") );

	struct stat st;
	CPPUNIT_ASSERT_EQUAL( stat( TESTDIR "safe.txt", &st), 0 );
	CPPUNIT_ASSERT( st.st_size > 0 );

	// Rewrite existing file
	CPPUNIT_ASSERT_NO_THROW( File::SafeWrite(TESTDIR "safe.txt", "Hello again", 0666));
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "safe.txt") );

	unlink( TESTDIR "safe.txt");

	// Safe write  stringlist
	CPPUNIT_ASSERT_NO_THROW( File::SafeWrite(TESTDIR "safe.txt", data, 0666));
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "safe.txt") );
	CPPUNIT_ASSERT_NO_THROW( verify = File::GetContent( TESTDIR "safe.txt") );
	CPPUNIT_ASSERT_EQUAL( data.size(), verify.size());

	unlink( TESTDIR "safe.txt");

	// Safe write buffer
	char buf[1024]={0x0, 0x10, 0x1a, 0x1f};
	CPPUNIT_ASSERT_NO_THROW( File::SafeWrite(TESTDIR "safe.txt", static_cast<void*>(buf), sizeof(buf), 0666));
	CPPUNIT_ASSERT( File::FileExists( TESTDIR "safe.txt") );

	CPPUNIT_ASSERT_EQUAL( stat( TESTDIR "safe.txt", &st), 0 );
	CPPUNIT_ASSERT_EQUAL( st.st_size, static_cast<long>(sizeof(buf)) );

	unlink( TESTDIR "safe.txt");
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

void TestFileUtils::testMkDir()
{
	//cout << "\ntestMkdir"<< endl;
	struct stat st;
	stringstream ss;

	ss << TESTDIR << "/tmpdir";

	mkdir( ss.str().c_str()  , 0700);

	ss << "/t1";
	string pdir = ss.str();
	File::MkDir( ss.str(), 0755);

	CPPUNIT_ASSERT( stat( ss.str().c_str(), &st ) == 0);
	CPPUNIT_ASSERT( (static_cast<int>(st.st_mode) & ~S_IFMT) == 0755 );

	ss << "/t2";
	File::MkPath( ss.str(), 0700 );
	CPPUNIT_ASSERT( stat( ss.str().c_str(), &st ) == 0);
	CPPUNIT_ASSERT( (static_cast<int>(st.st_mode) & ~S_IFMT) == 0700 );

	CPPUNIT_ASSERT( stat( pdir.c_str(), &st ) == 0);
	CPPUNIT_ASSERT( (static_cast<int>(st.st_mode) & ~S_IFMT) == 0755 );

}
