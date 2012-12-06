#ifndef TESTSHM_H_
#define TESTSHM_H_

#include <cppunit/extensions/HelperMacros.h>

class TestSHM: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestSHM );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void test();

};

#endif /* SOCKETTEST_H_ */
