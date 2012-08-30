/*
 * CircularBuffer.h
 *
 *  Created on: 27 August 2012
 *      Author: tor
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <vector>
#include <list>
#include <map>
#include <memory>

#include "Mutex.h"

using namespace std;

namespace Utils{

class CircularBuffer;
class CircularReader;

typedef shared_ptr<CircularReader> CircularReaderPtr;
typedef shared_ptr<void> CircularData;


class CircularReader{
private:
	CircularReader() = delete;
	CircularReader(const CircularReader& ) = delete;
	CircularReader& operator=(const CircularReader&) = delete;

	CircularReader(int readp, CircularBuffer& cbuf, int id);
public:
	list< CircularData > Read();
	virtual ~CircularReader();
	bool operator==(const CircularReader& cr) const;
private:
	int id;
	int rp; //Read pointer into buffer
	CircularBuffer& cbuf; // Back reference to our buffer
	friend class CircularBuffer;
};

class CircularBuffer{
private:
	CircularBuffer() = delete;
public:
	CircularBuffer(int capacity);

	void AddData(CircularData data);

	CircularReaderPtr GetReader();
	void PutReader(CircularReaderPtr rd);

	virtual ~CircularBuffer();
private:

	void UpdateReaders(void);

	Mutex mutex; // Mutex protecting pointer manipulations.
	int wp; // Write pointer
	int rp; // Internal read pointer used to initialize readers rp
	int datasize; // Precompute size of data member

	vector< CircularData > data;
	map< int, CircularReaderPtr > readers;
	int curCircularReaderID;
	friend class CircularReader;
};

}
#endif
