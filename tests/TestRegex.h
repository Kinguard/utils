#ifndef TESTREGEX_H_
#define TESTREGEX_H_

#include <cppunit/extensions/HelperMacros.h>

class TestRegex: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestRegex );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
};

#endif /* TESTREGEX_H_ */
