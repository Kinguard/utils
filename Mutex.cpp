#include "Mutex.h"
#include <stdexcept>
#include <cstring>
#include <cerrno>

namespace Utils{

Mutex::Mutex(int type){
	if(type){
		pthread_mutexattr_t   mta;
		int rc = pthread_mutexattr_init(&mta);
		if(rc){
			throw std::runtime_error("Unable to create mutex attributes : "+std::string(strerror(errno)));
		}
		
		if(type==MUTEX_RECURSIVE){
			if(pthread_mutexattr_settype(&mta,PTHREAD_MUTEX_RECURSIVE)){
				throw std::runtime_error("Unable to set recursive attribute on mutex : "+std::string(strerror(errno)));
			}
		}else if(type==MUTEX_ERRORCHECK){
			if(pthread_mutexattr_settype(&mta,PTHREAD_MUTEX_ERRORCHECK)){
				throw std::runtime_error("Unable to set recursive attribute on mutex : "+std::string(strerror(errno)));
			}
		}else{
			throw std::runtime_error("Illegal mutex type");
		}
		
		pthread_mutex_init(&lock,&mta);
		
		if(pthread_mutexattr_destroy(&mta)){
			throw std::runtime_error("Failed to destroy mutex init structure");
		}
		
	}else{
    	pthread_mutex_init(&lock,NULL);
	}
}

void Mutex::Lock(){
    pthread_mutex_lock(&lock);
}

void Mutex::Unlock(){
    pthread_mutex_unlock(&lock);
}

pthread_mutex_t* Mutex::GetPThreadMutex(){
    return &lock;
}

Mutex::~Mutex(){
}

}
