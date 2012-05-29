
#include "Condition.h"
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdio>

namespace Utils{

Condition::Condition(){
    pthread_cond_init(&condition,NULL);
}

void Condition::Wait(){
    lock.Lock();

    if (pthread_cond_wait(&condition, lock.GetPThreadMutex())) {
		throw std::runtime_error("Failed to wait on condition : "+std::string(strerror(errno)));
    }

    lock.Unlock();
}

void Condition::Notify(){
    lock.Lock();
    if (pthread_cond_signal(&condition)) {
		throw std::runtime_error("Failed to signal writer thread : "+std::string(strerror(errno)));
    }
    lock.Unlock();
}

void Condition::NotifyAll(){
    lock.Lock();
    if (pthread_cond_broadcast(&condition)) {
		throw std::runtime_error("Failed to signal writer thread : "+std::string(strerror(errno)));
    }
    lock.Unlock();
}

Condition::~Condition(){
}

}
