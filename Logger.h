/*
 * Logger.h
 *
 *  Created on: Sep 4, 2012
 *      Author: tor
 *
 *  Loosely modelled on stl stream classes
 */

/*
 * Compile with -lrt for time to work
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Exceptions.h"

#include <functional>
#include <iomanip>
#include <sstream>
#include <string>

#include <sys/time.h>

namespace Utils{

class Logger;

class Manip {
public:
	Manip(){}
	virtual void operator()(Logger& logger) = 0;
	virtual ~Manip(){}
};

class Logger {
private:
	std::function< void (const std::string&) > out;
	std::string name;
	bool timestamp;
	bool hasname;
	bool first;
	std::ostringstream oss;

	void doFirst(){
		if( this->first ) {
			if ( this->timestamp ){
				Logger::getTime();
				oss << " : ";
			}
			if( this->hasname ){
				oss << this->name << ", ";
			}

			this->first = false;
		}
	}

public:

	Logger() = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(const Logger&) = delete;

	Logger(std::function< void (const std::string&) > o, const std::string& name = "", bool timestamp = true):
		out(o),name(name), timestamp(timestamp), first(true)
	{
		this->hasname = name!="";
	}

	Logger& operator<<(const std::string& msg){
		this->doFirst();
		oss << msg;
		return *this;
	}

	Logger& operator<<(const int msg){
		this->doFirst();
		oss << msg;
		return *this;
	}


	Logger& operator<<(Manip& m){

		m(*this);

		return *this;
	}


	Logger& flush(){
		oss << std::endl;
		this->out(oss.str());
		oss.str("");
		this->first = true;
		return *this;
	}

	~Logger(){
		this->flush();
	}

	void getTime(void){
		timespec ts;
		if( clock_gettime(CLOCK_MONOTONIC_RAW, &ts) ){
			throw Utils::ErrnoException("Failed to get time");
		}
		oss.precision(6);
		oss << std::fixed<< ts.tv_sec + (ts.tv_nsec/1e9);
	}

};

class Endl: public Manip{
public:
	Endl(){}
	virtual void operator()(Logger& logger){
		logger.flush();
	}
	virtual ~Endl(){}
};

extern Endl end;


} // End Namespace Utils



#endif /* LOGGER_H_ */
