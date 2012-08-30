#ifndef TESTCircularBuffer_H_
#define TESTCircularBuffer_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCircularBuffer: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestCircularBuffer );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void test();

};

#endif /* CircularBuffer_H_ */
