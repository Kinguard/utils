#ifndef TESTLOGGER_H_
#define TESTLOGGER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestLogger: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestLogger );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST( testWriter );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testWriter();

	void test();

};

#endif /* SOCKETTEST_H_ */
