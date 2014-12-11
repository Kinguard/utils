#include "TestTimer.h"

#include "Timer.h"
#include <unistd.h>
CPPUNIT_TEST_SUITE_REGISTRATION ( TestTimer );

using namespace Utils;

void TestTimer::setUp()
{
}

void TestTimer::tearDown()
{
}

class TimerTest: public Timer
{
public:
	TimerTest():Timer(),counter(0)
	{
	}

	virtual void Event()
	{
		this->counter++;
	}

	virtual ~TimerTest(){}

	int getCounter() const
	{
		return counter;
	}

private:
	int counter;
};

void TestTimer::Test()
{
	CPPUNIT_ASSERT_NO_THROW( Timer() );

	TimerTest t;

	t.Start( Timer::Freq(200), Timer::Freq(200) );
	usleep(20000);
	t.Stop();
	usleep(10000); // Let timer stop

	int count = t.getCounter();

	CPPUNIT_ASSERT( count > 0 );
	usleep(10000);
	CPPUNIT_ASSERT_EQUAL( count, t.getCounter() );
}


static int counter = 0;
static void testcounter(int t)
{
	counter += t;
}

void TestTimer::TestCallback()
{
	CPPUNIT_ASSERT_NO_THROW( CallBackTimer(testcounter, 1) );

	CallBackTimer t(testcounter, 1);

	t.Start( Timer::Freq(200), Timer::Freq(200) );
	usleep(20000);
	t.Stop();
	usleep(10000); // Let timer stop

	int count = counter;

	CPPUNIT_ASSERT( count > 0 );
	usleep(10000);
	CPPUNIT_ASSERT_EQUAL( count, counter );
}


