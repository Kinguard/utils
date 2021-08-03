#include "TestThread.h"

#include "Thread.h"
#include "Process.h"

#include <unistd.h>

using namespace std;
using namespace Utils;

CPPUNIT_TEST_SUITE_REGISTRATION ( TestThread );

void TestThread::setUp()
{
}

void TestThread::tearDown()
{
}

static int rvp;

void SayHello(const string& name)
{
	(void) name;
	//cout << "Hello, "<<name << endl;
	rvp = 0;
}


class Tester
{
public:
	Tester():count(0){}
	Tester(const Tester&) = delete;

	void CallMe()
	{
		rvp = 0;
	}

	void SpeakValue(int i)
	{
		this->count = i;
		rvp = 0;
	}

	virtual ~Tester(){}

	int count;
};


static bool waitcompletion()
{
	int timeout=100;
	do
	{
		usleep(1);
	}while( rvp != 0 && --timeout != 0 );

	return timeout != 0;
}

void TestThread::Test()
{
	Thread::Function f1 = std::bind( SayHello, "Mello");

	rvp = 1;
	Thread::Async( &f1 );
	CPPUNIT_ASSERT( waitcompletion() );

	Tester t;

	Thread::Function f2 = std::bind( &Tester::CallMe, &t);
	rvp = 1;
	Thread::Async( &f2 );
	CPPUNIT_ASSERT( waitcompletion() );


	Thread::Function f3 = std::bind( &Tester::SpeakValue, &t, 3113);
	CPPUNIT_ASSERT_EQUAL( 0, t.count );
	rvp = 1;
	Thread::Async( &f3 );
	CPPUNIT_ASSERT( waitcompletion() );
	CPPUNIT_ASSERT_EQUAL( 3113, t.count );

}



void TestThread::TestAsync()
{
	static Utils::Thread::Function mountandlink = []()
	{
		Utils::Process::Exec( "echo hello>>/tmp/hello.txt; sleep 5; echo world>>/tmp/hello.txt");
	};

	Thread::Async( &mountandlink);
	Thread::Async( &mountandlink);
	Thread::Async( &mountandlink);
}
