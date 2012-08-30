#include "Thread.h"
#include "Exceptions.h"
#include <sys/signal.h>
#include <string>
#include <cstring>

namespace Utils{

Thread::Thread(bool detached): thread(0), detached(detached),running(false){

}

void* Thread::BootstrapThread(void* obj)
{
    Thread* q=static_cast<Thread*>(obj);
    if (q) {
    	q->running=true;
	    q->Run();
		q->running=false;
    }
    return NULL;

}

bool Thread::isRunning()
{
	return this->running;
}

void Thread::Start()
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

void Thread::Signal(int signum)
{
    pthread_kill( this->thread, signum );
}

Thread::~Thread()
{
	pthread_attr_destroy(&this->attr);
}

}
