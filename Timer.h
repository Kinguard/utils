/*

	libutils - a collection of usable tools

	Copyright (c) 2015 Tor Krill <tor@openproducts.com>

	This file is part of libutils

	libutils is  free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

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

	struct sigevent sev{};
	timer_t tid{};
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
