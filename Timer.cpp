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

#include "Timer.h"
#include "Exceptions.h"

#include <stdexcept>
#include <iostream>
#include <utility>

using namespace std;

namespace Utils {

Timer::Timer(): running(false)
{
	this->sev.sigev_notify = SIGEV_THREAD;
	this->sev.sigev_notify_function = Timer::callback;
	this->sev.sigev_notify_attributes = nullptr;
	this->sev.sigev_value.sival_ptr = this;

	if( timer_create( CLOCK_MONOTONIC, &this->sev, &this->tid) == -1 )
	{
		throw ErrnoException("Failed to create timer");
	}
}

void Timer::Start(uint64_t start, uint64_t interval)
{
	struct itimerspec its{};

	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = start;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = interval;

	if( timer_settime( this->tid, 0, &its, nullptr)  == -1 )
	{
		throw ErrnoException("Failed to set timer");
	}
	this->running = ( start != 0 );
}

void Timer::Stop()
{
	this->Start(0,0);
	this->running = false;
}

uint64_t Timer::GetTime()
{
	struct itimerspec it{};

	if( timer_gettime( this->tid, &it) == -1 )
	{
		throw ErrnoException("Unable to get time");
	}

	return it.it_value.tv_nsec;
}

void Timer::Event()
{
}

Timer::~Timer()
{
	if( this->running )
	{
		try
		{
			this->Stop();
		}
		catch (exception& e)
		{
			(void) e;
		}
	}
}

void Timer::callback(union sigval sv)
{
	Timer* t = static_cast<Timer*>( sv.sival_ptr );
	t->Event();
}



CallBackTimer::CallBackTimer(std::function<void(int)> func, int val): Timer(), callback(std::move(func)), value(val)
{

}

void CallBackTimer::Event()
{
	this->callback( this->value );
}

CallBackTimer::~CallBackTimer()
= default;

}
