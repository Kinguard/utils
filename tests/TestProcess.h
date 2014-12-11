#ifndef TESTPROCESS_H_
#define TESTPROCESS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestProcess: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestProcess );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
};

#endif /* TESTPROCESS_H_ */
