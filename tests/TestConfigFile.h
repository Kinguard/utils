#ifndef TESTCONFIGFILE_H_
#define TESTCONFIGFILE_H_

#include <cppunit/extensions/HelperMacros.h>

class TestConfigFile: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestConfigFile );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
};

#endif /* TESTCONFIGFILE_H_ */
