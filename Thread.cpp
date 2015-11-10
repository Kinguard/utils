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

#include "Thread.h"
#include "Exceptions.h"
#include <sys/signal.h>
#include <string>
#include <cstring>

namespace Utils{

Thread::Thread(bool detached): thread(0), detached(detached),state( UnInitialized)
{
	int s = pthread_attr_init(&attr);
	if( s != 0 ){
		throw ErrnoException("Failed to init thread attribute");
	}

	if( this->detached ){
		s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if( s != 0 ){
			throw ErrnoException("Failed to set detached state on thread attribute");
		}
	}
}

void* Thread::BootstrapThread(void* obj)
{
	Thread* q=static_cast<Thread*>(obj);
	if (q)
	{
		q->state = Initialized;
		q->PreRun();
		q->state = Running;
		q->Run();
		q->state = HasRun;
		q->PostRun();
		q->state = Terminated;
	}
	return NULL;

}

bool Thread::isRunning()
{
	return this->state == Running;
}

void Thread::Start()
{
    pthread_create( &this->thread, &this->attr, Thread::BootstrapThread, this);
}

void Thread::Kill()
{
    this->Signal(SIGKILL);
}

void Thread::Yield()
{
	pthread_yield();
}

static void* trampolin(void*  arg)
{
	Thread::Function* fn = static_cast<Thread::Function*>(arg);

	if( fn )
	{
		(*fn)();
	}
}

void Thread::Async(Function *fn)
{
	pthread_t tid;
	pthread_attr_t attr;

	if( pthread_attr_init(&attr) != 0 )
	{
		throw ErrnoException("Failed to initialize thread attributes");
	}

	if( pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_DETACHED ) != 0 )
	{
			throw ErrnoException("Failed to set thread attribute detached");
	}

	if( pthread_create( &tid, &attr, trampolin, fn) != 0 )
	{
		throw ErrnoException("Failed to create thread");
	}
}

void Thread::Signal(int signum)
{
    pthread_kill( this->thread, signum );
}

int Thread::Join()
{
	return pthread_join( this->thread, NULL);
}

Thread::State Thread::RunState()
{
	return this->state;
}

Thread::~Thread()
{
	pthread_attr_destroy(&this->attr);
}

}
