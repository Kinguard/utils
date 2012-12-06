/*
 * Shm.h
 *
 *  Created on: Dec 6, 2012
 *      Author: tor
 */

#ifndef SHM_H_
#define SHM_H_

#include <string>
#include <sys/ipc.h>

#include "Exceptions.h"

namespace Utils {

class Shm {
public:
	Shm(const std::string& path, size_t size, int token = 123 );
	void Remove();
	void* operator*();
	void* Value();
	virtual ~Shm();
private:
	int shmid;
	key_t token;
	void* data;
};

} /* namespace Utils */
#endif /* SHM_H_ */
