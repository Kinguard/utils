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

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <vector>
#include <list>
#include <map>
#include <memory>

#include "Condition.h"
#include "Mutex.h"

using namespace std;

namespace Utils{

class CircularBuffer;
class CircularReader;

typedef shared_ptr<CircularBuffer> CircularBufferPtr;
typedef shared_ptr<CircularReader> CircularReaderPtr;
typedef struct {
	size_t size;
	shared_ptr<void> data;
}CircularData;


class CircularReader{
private:
	CircularReader() = delete;
	CircularReader(const CircularReader& ) = delete;
	CircularReader& operator=(const CircularReader&) = delete;

	CircularReader(int readp, CircularBuffer& cbuf, int id, bool eof);
public:
	bool Read(list< CircularData >& data);
	virtual ~CircularReader();
	bool operator==(const CircularReader& cr) const;
	bool Empty();
	bool Eof();
private:
	int id;
	int rp; //Read pointer into buffer
	CircularBuffer& cbuf; // Back reference to our buffer
	bool eof;
	friend class CircularBuffer;
};

class CircularBuffer{
private:
	CircularBuffer() = delete;
public:
	CircularBuffer(int capacity);

	void AddData(CircularData data);
	void SignalReaders(void); // Tell readers we have new data
	void SetEof(); // Set up eof and inform readers

	CircularReaderPtr GetReader();
	void PutReader(CircularReaderPtr rd);
	int NumReaders();

	virtual ~CircularBuffer();
private:

	void UpdateReaders(void);

	void WaitForData();

	Mutex mutex; // Mutex protecting pointer manipulations.
	int wp; // Write pointer
	int rp; // Internal read pointer used to initialize readers rp
	int datasize; // Precompute size of data member

	Condition hasdata;

	vector< CircularData > data;
	map< int, CircularReaderPtr > readers;
	int curCircularReaderID;
	bool eof;
	friend class CircularReader;
};

}
#endif
