#ifndef MYCONDITION_H
#define MYCONDITION_H

#include <pthread.h>
#include "Mutex.h"
#include "Thread.h"

namespace Utils{

class Condition{
private:
    Mutex lock;
    pthread_cond_t condition;
public:
    Condition();
    void Wait();
    void Notify();
    void NotifyAll();
    virtual ~Condition();

};

}

#endif
