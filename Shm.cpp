/*
 * Shm.cpp
 *
 *  Created on: Dec 6, 2012
 *      Author: tor
 */

#include "Shm.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#include "FileUtils.h"

namespace Utils {

using namespace std;

Shm::Shm(const string& path, size_t size, int token)
{
	if ( ! File::FileExists( path ) )
	{
		throw std::runtime_error( "IPC file "+ path + " doesn't exist" );
	}

	if ( ( this->token = ftok( path.c_str(), token ) ) == -1 )
	{
		throw ErrnoException("Unable to create ipc token");
	}

	if ( ( this->shmid = shmget(this->token, size, 0644 | IPC_CREAT)) == -1 )
	{
		throw ErrnoException("Failed to allocate shared memory segment");
	}


	this->data = shmat(this->shmid, NULL, 0);

	if ( this->data == (char *) (-1) )
	{
		throw ErrnoException("Failed to attach to shared memory segment");
    }

}

void Shm::Remove()
{
	if ( shmctl( this->shmid, IPC_RMID, NULL ) == -1 )
	{
		throw ErrnoException("Failed to remove shared memory segment");
	}
}

void* Shm::operator *()
{
	return this->Value();
}

void* Shm::Value()
{
	return this->data;
}

Shm::~Shm()
{
	if (shmdt(data) == -1)
	{
		throw ErrnoException("Failed to detach shared memory segment");
	}
}

} /* namespace Utils */
