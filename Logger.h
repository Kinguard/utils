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
