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
