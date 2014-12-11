#ifndef TESTTIMER_H_
#define TESTTIMER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestTimer: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestTimer );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST( TestCallback );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
	void TestCallback();
};

#endif /* TESTTIMER_H_ */
