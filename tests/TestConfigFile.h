#ifndef TESTCONFIGFILE_H_
#define TESTCONFIGFILE_H_

#include <cppunit/extensions/HelperMacros.h>

class TestConfigFile: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestConfigFile );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST( TestIni );
	CPPUNIT_TEST( TestIniS3ql );
	CPPUNIT_TEST( TestIniFile );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
	void TestIni();
	void TestIniS3ql();
	void TestIniFile();
};

#endif /* TESTCONFIGFILE_H_ */
