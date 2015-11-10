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
