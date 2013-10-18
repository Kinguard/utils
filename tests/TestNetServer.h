#ifndef TESTNETSERVER_H_
#define TESTNETSERVER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestNetServer: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestNetServer );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void Test();
};

#endif /* TESTNETSERVER_H_ */
