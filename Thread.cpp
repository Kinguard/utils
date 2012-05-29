
#include "Thread.h"
#include <sys/signal.h>
#include <stdexcept>
#include <string>
#include <cerrno>
#include <cstring>

namespace Utils{

Thread::Thread(bool detached):detached(detached),running(false){

}

void* Thread::BootstrapThread(void* obj){
    Thread* q=static_cast<Thread*>(obj);
    if (q) {
    	q->running=true;
	    q->Run();
		q->running=false;
    }
    return NULL;

}

bool Thread::isRunning(){
	return this->running;
}

void Thread::Start(){

	int s = pthread_attr_init(&attr);
	if( s != 0 ){
		throw std::runtime_error("Failed to init thread attribute : "+std::string(strerror(errno)));
	}

	if( this->detached ){
		s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if( s != 0 ){
			throw std::runtime_error("Failed to set detached state on thread attribute :" +std::string(strerror(errno)));
		}
	}

    pthread_create(&this->thread,&this->attr,Thread::BootstrapThread,this);

}

void Thread::Kill(){
    Signal(SIGKILL);
}

void Thread::Yield(){
	pthread_yield();
}

void Thread::Signal(int signum){
    pthread_kill(thread,signum);
}

Thread::~Thread(){
}

}
