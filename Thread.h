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
