#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <pthread.h>

namespace Utils{

class Thread{
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
	bool running;

    static void* BootstrapThread(void* inst);

protected:

public:
    Thread(bool detached=true);

    bool isRunning();

    virtual void Start();

    virtual void PreRun() {}

    virtual void Run(){};

    virtual void PostRun() {}

    virtual int Join();

    virtual void Signal(int signum);

    virtual void Kill();

    static void Yield();

    virtual ~Thread();

};

}
#endif
