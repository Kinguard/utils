#ifndef MYLOCK_H
#define MYLOCK_H

#include <pthread.h>

#include "ClassTools.h"

namespace Utils{

class Condition;

#define MUTEX_NORMAL 0
#define MUTEX_RECURSIVE 1
#define MUTEX_ERRORCHECK 2

class Mutex{
private:
    pthread_mutex_t lock;
protected:
    pthread_mutex_t* GetPThreadMutex();
public:
    Mutex(int type=MUTEX_NORMAL);
    void Lock();
    void Unlock();
    virtual ~Mutex();
    friend class Condition;
};

class ScopedLock: public NoCopy{
private:
	Mutex& m;
public:
	ScopedLock(Mutex& mt);
	virtual ~ScopedLock();

};

}

#endif
