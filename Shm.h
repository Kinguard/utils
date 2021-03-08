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

#ifndef SHM_H_
#define SHM_H_

#include <string>
#include <sys/ipc.h>

#include "Exceptions.h"

namespace Utils {

class Shm {
public:
	Shm(const std::string& path, size_t size, int perm = 0644, int token = 123 );
	void Remove();
	void* operator*();
	void* Value();
	virtual ~Shm() = default;
private:
	int shmid;
	key_t token;
	void* data;
};

} /* namespace Utils */
#endif /* SHM_H_ */
