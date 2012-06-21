#include "Condition.h"
#include "Exceptions.h"

namespace Utils{

Condition::Condition(){
    pthread_cond_init(&condition,NULL);
}

void Condition::Wait(){
    lock.Lock();

    if (pthread_cond_wait(&condition, lock.GetPThreadMutex())) {
		throw ErrnoException("Failed to wait on condition");
    }

    lock.Unlock();
}

void Condition::Notify(){
    lock.Lock();
    if (pthread_cond_signal(&condition)) {
		throw ErrnoException("Failed to signal condition");
    }
    lock.Unlock();
}

void Condition::NotifyAll(){
    lock.Lock();
    if (pthread_cond_broadcast(&condition)) {
		throw ErrnoException("Failed to broadcast condition");
    }
    lock.Unlock();
}

Condition::~Condition(){
}

}
