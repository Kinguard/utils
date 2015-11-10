/*

	libutils - a collection of usable tools

	Copyright (c) 2015 Tor Krill <tor@openproducts.com>

	This file is part of libutils

	libutils is  free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Shm.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#include "FileUtils.h"

namespace Utils {

using namespace std;

Shm::Shm(const string& path, size_t size, int perm, int token)
{
	if ( ! File::FileExists( path ) )
	{
		File::Write( path, "", perm );
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
