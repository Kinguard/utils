#ifndef TESTNETUTILS_H_
#define TESTNETUTILS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestNetUtils: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestNetUtils );
	CPPUNIT_TEST( testGetHWAddr );
	CPPUNIT_TEST( testGetNetmask );
	CPPUNIT_TEST( testGetIfAddr );
	CPPUNIT_TEST( testGetInterfaces );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testGetHWAddr();
	void testGetNetmask();
	void testGetIfAddr();
	void testGetInterfaces();
};

#endif /* SOCKETTEST_H_ */
