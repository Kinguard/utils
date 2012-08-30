#include "Condition.h"
#include "Exceptions.h"

namespace Utils{

Condition::Condition()
{
    pthread_cond_init(&this->condition,NULL);
}

void Condition::Wait()
{
	this->lock.Lock();

    if (pthread_cond_wait(&this->condition, this->lock.GetPThreadMutex())) {
        this->lock.Unlock();
		throw ErrnoException("Failed to wait on condition");
    }

    this->lock.Unlock();
}

void Condition::Notify()
{
	this->lock.Lock();
    if (pthread_cond_signal(&this->condition)) {
        this->lock.Unlock();
		throw ErrnoException("Failed to signal condition");
    }
    this->lock.Unlock();
}

void Condition::NotifyAll()
{
	this->lock.Lock();
    if (pthread_cond_broadcast(&this->condition)) {
        this->lock.Unlock();
		throw ErrnoException("Failed to broadcast condition");
    }
    this->lock.Unlock();
}

Condition::~Condition()
{
	pthread_cond_destroy(&this->condition);
}

}
