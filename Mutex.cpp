#include "Mutex.h"
#include "Exceptions.h"
#include <stdexcept>

namespace Utils{

Mutex::Mutex(int type){
	if(type){
		pthread_mutexattr_t   mta;
		int rc = pthread_mutexattr_init(&mta);
		if(rc){
			throw ErrnoException("Unable to create mutex attributes");
		}

		if(type==MUTEX_RECURSIVE){
			if(pthread_mutexattr_settype(&mta,PTHREAD_MUTEX_RECURSIVE)){
				pthread_mutexattr_destroy(&mta);
				throw ErrnoException("Unable to set recursive attribute on mutex");
			}
		}else if(type==MUTEX_ERRORCHECK){
			if(pthread_mutexattr_settype(&mta,PTHREAD_MUTEX_ERRORCHECK)){
				pthread_mutexattr_destroy(&mta);
				throw ErrnoException("Unable to set error check attribute on mutex");
			}
		}else{
			pthread_mutexattr_destroy(&mta);
			throw std::runtime_error("Illegal mutex type");
		}

		pthread_mutex_init(&this->lock,&mta);

		if(pthread_mutexattr_destroy(&mta)){
			throw std::runtime_error("Failed to destroy mutex init structure");
		}

	}else{
		pthread_mutex_init(&this->lock,NULL);
	}
}

void Mutex::Lock(){
    pthread_mutex_lock(&this->lock);
}

void Mutex::Unlock(){
    pthread_mutex_unlock(&this->lock);
}

pthread_mutex_t* Mutex::GetPThreadMutex(){
    return &this->lock;
}

Mutex::~Mutex(){
	pthread_mutex_destroy(&this->lock);
}

ScopedLock::ScopedLock ( Mutex& mt ): m(mt)
{
	m.Lock();
}

ScopedLock::~ScopedLock ()
{
	m.Unlock();
}


}
