#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

#include <time.h>
#include <signal.h>
#include <stdint.h>

namespace Utils {

class Timer
{
public:
	Timer();

	Timer( const Timer& ) = delete;
	Timer& operator=(const Timer& ) = delete;

	void Start(uint64_t ns_start, uint64_t ns_interval);
	void Stop();
	uint64_t GetTime();

	virtual void Event();

	virtual ~Timer();

	static uint64_t Freq(int f)
	{
		return 1000000000/f;
	}

private:
	bool running;
	static void callback(union sigval sv);

	struct sigevent sev;
	timer_t tid;
};

class CallBackTimer: public Timer
{
public:
	CallBackTimer(std::function<void(int)> func , int val);

	virtual void Event();

	virtual ~CallBackTimer();
private:
	std::function<void(int)> callback;
	int value;
};

}
#endif
