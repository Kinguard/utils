#ifndef TESTFILEUTILS_H_
#define TESTFILEUTILS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestFileUtils: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestFileUtils );
	CPPUNIT_TEST( testFileExists );
	CPPUNIT_TEST( testDirExists );
	CPPUNIT_TEST( testFileWrite );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testFileExists();
	void testDirExists();
	void testFileWrite();
//Todo: make rest of tests
};

#endif /* SOCKETTEST_H_ */