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

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <pthread.h>
#include <functional>

namespace Utils{

class Thread{

public:
	enum State {
		UnInitialized,
		Initialized,
		Running,
		HasRun,
		Terminated
	};

	typedef std::function<void()> Function;

private:
	/**
	* Threadid for worker thread.
	*/
	pthread_t thread;

	/*
	* Attributes for thread
	*/
	pthread_attr_t attr;

	/*
	* Should thread be detached or joinable?
	*/
	bool detached;
	/*
	* Is thread running/alive
	*/

	static void* BootstrapThread(void* inst);

	Thread::State state;

protected:

public:

	Thread(bool detached=true);

	bool isRunning();

	Thread::State RunState();

	virtual void Start();

	virtual void PreRun() {}

	virtual void Run(){}

	virtual void PostRun() {}

	virtual int Join();

	virtual void Signal(int signum);

	virtual void Kill();

	static void Yield();

	static void Async(Function* fn);

	virtual ~Thread();

};

}
#endif
