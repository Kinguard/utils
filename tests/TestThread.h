#ifndef TESTTHREAD_H_
#define TESTTHREAD_H_

#include <cppunit/extensions/HelperMacros.h>

class TestThread: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestThread );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void Test();
};

#endif /* TESTTHREAD_H_ */