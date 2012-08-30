/*
 * CircularBuffer.cpp
 *
 *  Created on: 27 August 2012
 *      Author: tor
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

Utils::CircularReader::CircularReader(int readp, CircularBuffer& cbuf,int id):
		id(id), rp(readp), cbuf(cbuf)
{

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


list< CircularData > Utils::CircularReader::Read() {
	list< CircularData > l;
	this->cbuf.mutex.Lock();

	if ( this->rp == this->cbuf.wp ){
		//No data atm, wait for it
		this->cbuf.mutex.Unlock();
		this->cbuf.WaitForData();
		this->cbuf.mutex.Lock();
	}

	while( this->rp != this->cbuf.wp ){
		l.push_back(this->cbuf.data[this->rp]);

		this->rp = (this->rp +1 )% this->cbuf.datasize;
	}

	this->cbuf.mutex.Unlock();
	return l;
}

Utils::CircularReader::~CircularReader()
{

}

/*
 *
 * BUFFER
 *
 */

Utils::CircularBuffer::CircularBuffer(int capacity):
		wp(0), rp(0), datasize(capacity+1), data(capacity+1), curCircularReaderID(0)
{
}

void Utils::CircularBuffer::UpdateReaders(void) {

	for(auto& x: this->readers){
		CircularReaderPtr r = x.second;

		if( r->rp  == this->wp){
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

	CircularReaderPtr reader = CircularReaderPtr( new CircularReader(this->rp, *this, newid) );
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

void Utils::CircularBuffer::WaitForData()
{
	this->hasdata.Wait();
}


Utils::CircularBuffer::~CircularBuffer()
{
}
