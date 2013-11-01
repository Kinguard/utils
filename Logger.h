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
#include <iostream>
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
public:

	enum LogLevel{
		Emerg,
		Alert,
		Crit,
		Error,
		Warning,
		Notice,
		Info,
		Debug
	};

	Logger() = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(const Logger&) = delete;

	Logger(std::function<void(const std::string&)> o, const std::string& name = "", bool timestamp = true);

	Logger& operator <<(const std::string& msg);

	Logger& operator <<(const int msg);

	Logger& operator <<(Manip& m);

	Logger& operator <<(LogLevel lev);

	Logger& flush();

	void SetLevel(LogLevel level);

	void SetLogName(const std::string& name);

	void SetTimeStamping(bool dotimestamp);

	void SetOutputter(std::function<void(const std::string&)> o);

	~Logger();

	void getTime(void);
private:
	std::function< void (const std::string&) > out;
	std::string name;
	bool timestamp;
	bool hasname;
	bool first;
	std::ostringstream oss;
	LogLevel level,lastlevel;
	void doFirst();
};

class Endl: public Manip{
public:
	Endl(){}

	virtual void operator()(Logger& logger)
	{
		logger.flush();
	}

	virtual ~Endl(){}
};

extern Endl lend;
extern Logger logg;

class ClassWriter
{
public:
	ClassWriter();

	ClassWriter(const char* name, bool life = false);

	virtual void o(const char* line);

	virtual ~ClassWriter();

private:
	std::string name;
	bool lifecycle;
};

} // End Namespace Utils



#endif /* LOGGER_H_ */
