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

class Endl: public Manip{
public:
	Endl(){}
	virtual void operator()(Logger& logger);
	virtual ~Endl(){}
};

Endl end;

class Logger {
private:
	std::ostream& s;
	std::string name;
	bool hasname;
	bool first;
	std::ostringstream oss;
public:

	Logger() = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(const Logger&) = delete;

	Logger(std::ostream& os, const std::string& name=""): s(os),name(name), first(true)
	{
		this->hasname = name!="";
	}

	Logger& operator<<(const std::string& msg){
		if( first ) {
			Logger::getTime();
			oss << " : ";

			if( this->hasname ){
				oss << this->name << ", ";
			}

			first = false;
		}
		oss << msg;
		return *this;
	}

	Logger& operator<<(Manip& m){

		m(*this);

		return *this;
	}


	Logger& flush(){
		oss << std::endl;
		s << oss.str();
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
			throw std::runtime_error("Failed to get time");
		}
		oss.precision(6);
		oss << std::fixed<< ts.tv_sec + (ts.tv_nsec/1e9);
	}

};

void Endl::operator()(Logger& logger)
{
	logger.flush();
}


} // End Namespace Utils



#endif /* LOGGER_H_ */
