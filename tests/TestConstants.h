#ifndef TESTCONSTANTS_H_
#define TESTCONSTANTS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestConstants: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestConstants );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void test();

};

#endif /* TESTCONSTANTS_H_ */
