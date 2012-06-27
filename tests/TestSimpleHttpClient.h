#ifndef SOCKETTEST_H_
#define SOCKETTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class TestSimpleHttpClient: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestSimpleHttpClient );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void test();

};

#endif /* SOCKETTEST_H_ */
