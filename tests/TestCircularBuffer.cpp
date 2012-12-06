#include "TestCircularBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestCircularBuffer);

#include "CircularBuffer.h"

using namespace std;
using namespace Utils;

typedef struct _TSBuffer{
	int value;
	char data[184];
	//~_TSBuffer(){ cout << "im done!!"<<endl;}
} TSBuffer;

void TestCircularBuffer::setUp(){
}

void TestCircularBuffer::tearDown(){
}

void TestCircularBuffer::test()
{
	CircularBuffer cb(5);

	CircularReaderPtr r3 = cb.GetReader();

	{
		CircularReaderPtr r = cb.GetReader();

		for( int i=0; i<10; i++){
			CircularData item;
			item.data = shared_ptr<void>( new TSBuffer );
			item.size = sizeof( TSBuffer );

			TSBuffer* it = (TSBuffer*)item.data.get();
			it->value = i;

			cb.AddData(item);
		}
		list<CircularData> data = r->Read();

		CPPUNIT_ASSERT_EQUAL(5, (int)data.size());

		// Verify that we have the last 5 elements
		int i = 5;
		for(auto x: data){
			TSBuffer* it = (TSBuffer*) x.data.get();
			CPPUNIT_ASSERT( it->value == i++);
			//cout << it->value <<endl;
		}

		// Should be empty
		CPPUNIT_ASSERT_EQUAL(true,  r->Empty());

		cb.PutReader(r);
	}

	{
		// Get second reader
		CircularReaderPtr r2 = cb.GetReader();
		list<CircularData> d2 = r2->Read();

		CPPUNIT_ASSERT_EQUAL(5, (int)d2.size());

		int i = 5;
		for(auto x2: d2){
			TSBuffer* it = (TSBuffer*) x2.data.get();
			CPPUNIT_ASSERT( it->value == i++);
			//cout << it->value <<endl;
		}
	}
	{
		// Check last reader
		list<CircularData> d3 = r3->Read();

		CPPUNIT_ASSERT_EQUAL(5, (int)d3.size());

		int i = 5;
		for(auto x3: d3){
			TSBuffer* it = (TSBuffer*) x3.data.get();
			CPPUNIT_ASSERT( it->value == i++);
			//cout << it->value <<endl;
		}
	}
}
