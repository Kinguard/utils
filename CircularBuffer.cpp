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

#include "CircularBuffer.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace Utils;

/*
 *
 * READER
 *
 */

Utils::CircularReader::CircularReader(int readp, CircularBuffer& cbuf,int id, bool eof):
		id(id), rp(readp), cbuf(cbuf), eof(eof)
{
	// cout << "Circular reader created:"<<this<<endl;
}

bool Utils::CircularReader::operator==(const CircularReader& cr) const
{
	return this->id == cr.id;
}

bool Utils::CircularReader::Empty()
{
	this->cbuf.mutex.Lock();
	bool empty = this->rp == this->cbuf.wp;
	this->cbuf.mutex.Unlock();
	return empty;
}


bool Utils::CircularReader::Read(list< CircularData >& l) {

	if( this->eof )
	{
		return false;
	}

	this->cbuf.mutex.Lock();

	if ( this->rp == this->cbuf.wp ){
		//No data atm, wait for it
		this->cbuf.mutex.Unlock();

		this->cbuf.WaitForData();

		// Got eof while waiting?
		if( this->eof )
		{
			return false;
		}

		this->cbuf.mutex.Lock();
	}

	while( this->rp != this->cbuf.wp ){
		l.push_back(this->cbuf.data[this->rp]);

		this->rp = (this->rp +1 )% this->cbuf.datasize;
	}

	this->cbuf.mutex.Unlock();

	return true;
}

bool Utils::CircularReader::Eof()
{
	return this->eof;
}

Utils::CircularReader::~CircularReader()
{
	//cout << "Circular reader destroyed:"<<this<<endl;
}

/*
 *
 * BUFFER
 *
 */

Utils::CircularBuffer::CircularBuffer(int capacity):
		wp(0), rp(0), datasize(capacity+1), data(capacity+1), curCircularReaderID(0),
		eof(false)
{
}

void Utils::CircularBuffer::UpdateReaders(void) {
	static int ofcount = 0;
	for(auto& x: this->readers){
		CircularReaderPtr r = x.second;

		if( r->rp  == this->wp){
			if( ( ++ofcount % 250 ) == 0 ){
				cout << "!"<<flush;
			}
			r->rp = (r->rp + 1 ) % this->datasize;
		}
	}

	// Update internal RP
	if( this->rp == this->wp ){
		this->rp = (this->rp +1 ) % this->datasize;
	}
}

void Utils::CircularBuffer::AddData(CircularData data) {
	this->mutex.Lock();


	this->data[this->wp]=data;
	this->wp = (this->wp + 1 ) % this->datasize;

	this->UpdateReaders();

	this->mutex.Unlock();
}

CircularReaderPtr Utils::CircularBuffer::GetReader() {
	this->mutex.Lock();

	int newid = this->curCircularReaderID++;

	CircularReaderPtr reader = CircularReaderPtr(
			new CircularReader(this->rp, *this, newid, this->eof ) );

	this->readers[newid] = reader;

	this->mutex.Unlock();

	return reader;
}

void Utils::CircularBuffer::PutReader(CircularReaderPtr rd)
{
	this->mutex.Lock();


	if( this->readers.find(rd->id) == this->readers.end()  ){
		this->mutex.Unlock();
		throw runtime_error("Reader not found");
	}

	this->readers.erase(rd->id);

	this->mutex.Unlock();
}

void Utils::CircularBuffer::SignalReaders(void)
{
	this->hasdata.NotifyAll();
}

void Utils::CircularBuffer::SetEof()
{
	this->mutex.Lock();

	this->eof = true;

	// Set all readers to EOF
	for(auto& x: this->readers)
	{
		CircularReaderPtr r = x.second;

		r->eof = true;
	}

	this->mutex.Unlock();

	// Wake up any blocked readers
	this->SignalReaders();
}

int Utils::CircularBuffer::NumReaders()
{
	return this->readers.size();
}

void Utils::CircularBuffer::WaitForData()
{
	this->hasdata.Wait();
}


Utils::CircularBuffer::~CircularBuffer()
{
}
