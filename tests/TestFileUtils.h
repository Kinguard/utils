#ifndef TESTFILEUTILS_H_
#define TESTFILEUTILS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestFileUtils: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestFileUtils );
	CPPUNIT_TEST( testFileCopy );
	CPPUNIT_TEST( testFileExists );
	CPPUNIT_TEST( testFileSize );
	CPPUNIT_TEST( testDirExists );
	CPPUNIT_TEST( testLinkExists);
	CPPUNIT_TEST( testFileRead );
	CPPUNIT_TEST( testFileWrite );
	CPPUNIT_TEST( testRealPath );
    CPPUNIT_TEST( testMoveFile );
    CPPUNIT_TEST( testMoveDir );
	CPPUNIT_TEST( testMkDir );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testFileCopy();
	void testFileSize();
	void testFileExists();
	void testDirExists();
	void testLinkExists();
	void testFileRead();
	void testFileWrite();
	void testRealPath();
    void testMoveFile();
    void testMoveDir();
	void testMkDir();
//Todo: make rest of tests
};

#endif /* SOCKETTEST_H_ */
