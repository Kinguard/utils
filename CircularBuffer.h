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

	CircularReader(int readp, CircularBuffer& cbuf, int id);
public:
	list< CircularData > Read();
	virtual ~CircularReader();
	bool operator==(const CircularReader& cr) const;
	bool Empty();
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
	void SignalReaders(void); // Tell readers we have new data

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
	friend class CircularReader;
};

}
#endif
