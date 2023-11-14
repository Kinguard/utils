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
	TimerTest():Timer()
	{
	}

	void Event() override
	{
		this->counter++;
	}

	~TimerTest() override = default;

	[[nodiscard]] int getCounter() const
	{
		return counter;
	}

private:
	int counter{0};
};

void TestTimer::Test()
{
	CPPUNIT_ASSERT_NO_THROW( Timer() );

	TimerTest t;

	t.Start( Timer::Freq(200), Timer::Freq(200) );
	usleep(20000);
	t.Stop();
	usleep(10000); // Let timer stop

	int const count = t.getCounter();

	CPPUNIT_ASSERT( count > 0 );
	usleep(10000);
	CPPUNIT_ASSERT_EQUAL( count, t.getCounter() );
}


namespace {
int counter = 0;

void testcounter(int t)
{
	counter += t;
}

}

void TestTimer::TestCallback()
{
	CPPUNIT_ASSERT_NO_THROW( CallBackTimer(testcounter, 1) );

	CallBackTimer t(testcounter, 1);

	t.Start( Timer::Freq(200), Timer::Freq(200) );
	usleep(20000);
	t.Stop();
	usleep(10000); // Let timer stop

	int const count = counter;

	CPPUNIT_ASSERT( count > 0 );
	usleep(10000);
	CPPUNIT_ASSERT_EQUAL( count, counter );
}


