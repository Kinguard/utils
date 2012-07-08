#ifndef STRINGTEST_H_
#define STRINGTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class StringTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( StringTest );
	CPPUNIT_TEST( testTrimmed );
	CPPUNIT_TEST( testToUpper );
	CPPUNIT_TEST( testToLower );
	CPPUNIT_TEST( testSplit );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testTrimmed();
	void testToUpper();
	void testToLower();
	void testSplit();
};

#endif /* SOCKETTEST_H_ */
