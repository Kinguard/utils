#include "StringTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (StringTest);

#include "String.h"

using namespace Utils;

void StringTest::setUp(){
}

void StringTest::tearDown(){
}

void StringTest::testTrimmed()
{
	CPPUNIT_ASSERT_EQUAL(string("aaa"), String::Trimmed("  aaa   "," "));
	CPPUNIT_ASSERT_EQUAL(string("aaa"), String::Trimmed("aaa   "," "));
	CPPUNIT_ASSERT_EQUAL(string("aaa"), String::Trimmed("  aaa"," "));
	CPPUNIT_ASSERT_EQUAL(string("aa a"), String::Trimmed("  aa a   "," "));
	CPPUNIT_ASSERT(string("  aa a") !=  String::Trimmed("  aa a"," "));

	//cout <<"["<< string("aa a") << "] : ["<< String::Trimmed("  aa a   "," ")<<"]"<<endl;
}

void StringTest::testToUpper()
{
	CPPUNIT_ASSERT_EQUAL(string("ABCD"), String::ToUpper("abcd"));
	CPPUNIT_ASSERT_EQUAL(string("ABCD"), String::ToUpper("ABCD"));
	CPPUNIT_ASSERT_EQUAL(string("ABCD"), String::ToUpper("aBcD"));
	CPPUNIT_ASSERT_EQUAL(string("0ABCD1"), String::ToUpper("0abcd1"));
}

void StringTest::testToLower()
{
	CPPUNIT_ASSERT_EQUAL(string("abcd"), String::ToLower("ABCD"));
	CPPUNIT_ASSERT_EQUAL(string("abcd"), String::ToLower("abcd"));
	CPPUNIT_ASSERT_EQUAL(string("abcd"), String::ToLower("aBcD"));
	CPPUNIT_ASSERT_EQUAL(string("0abcd1"), String::ToLower("0ABCD1"));
}

void StringTest::testSplit()
{
	int items = String::Split(""," ").size();
	CPPUNIT_ASSERT_EQUAL(0, items);

	items = String::Split(" aaaa"," ").size();
	CPPUNIT_ASSERT_EQUAL(1, items);

	items = String::Split("aaaa "," ").size();
	CPPUNIT_ASSERT_EQUAL(1, items);

	items = String::Split("aaaa      "," ").size();
	CPPUNIT_ASSERT_EQUAL(1, items);

	items = String::Split(" a  a   a a      "," ").size();
	CPPUNIT_ASSERT_EQUAL(4, items);

	items = String::Split("XaXaXaXaX","X").size();
	CPPUNIT_ASSERT_EQUAL(4, items);

	items = String::Split("XaXXaXaXaXX","X").size();
	CPPUNIT_ASSERT_EQUAL(4, items);

	items = String::Split("XAaXAXAaXAaXAaXAXA","XA").size();
	CPPUNIT_ASSERT_EQUAL(4, items);

	list<string> res = String::Split(" 1 2  3 4"," ");
	CPPUNIT_ASSERT_EQUAL(string("1"),res.front());
	res.pop_front();

	CPPUNIT_ASSERT_EQUAL(string("2"),res.front());
	res.pop_front();

	CPPUNIT_ASSERT_EQUAL(string("3"),res.front());
	res.pop_front();

	CPPUNIT_ASSERT_EQUAL(string("4"),res.front());
	res.pop_front();

	CPPUNIT_ASSERT( res.empty() );

	CPPUNIT_ASSERT_EQUAL((size_t)2,String::Split("a b c d"," ",2).size());
	CPPUNIT_ASSERT_EQUAL((size_t)2,String::Split(" a b c d"," ",2).size());
	CPPUNIT_ASSERT_EQUAL((size_t)2,String::Split("a b c d "," ",2).size());
	CPPUNIT_ASSERT_EQUAL((size_t)2,String::Split("  a    b   c  d   "," ",2).size());
	CPPUNIT_ASSERT_EQUAL((size_t)3,String::Split(" a b c d "," ",3).size());
	CPPUNIT_ASSERT_EQUAL((size_t)4,String::Split("a b  c d "," ",4).size());

}

void StringTest::testUUID()
{
#if 0
	cout << "UUID: "<<String::UUID()<<endl;
#endif

	CPPUNIT_ASSERT_NO_THROW(String::UUID());
	string uuid=String::UUID();
	CPPUNIT_ASSERT( uuid.size() > 0 );
}
