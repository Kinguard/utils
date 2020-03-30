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

#include "Logger.h"

namespace Utils {
/* Global instances */
Endl lend;

Logger logg([](const std::string& s){ std::cout << s; },"",false);

/*
 * Implementation of Logger
 */

Logger::Logger(std::function<void(const std::string&)> o,
		const std::string& name, bool timestamp) :
		out(o), name(name), timestamp(timestamp), first(true),
		level(Logger::Error),lastlevel(Logger::Debug)
{
	this->hasname = name != "";
}

Logger&
Logger::operator << ( LogLevel lev )
{
	this->lastlevel = lev;
	return *this;
}

void
Logger::SetLevel ( LogLevel level )
{
	this->level = level;
}

Logger::LogLevel Logger::getLevel()
{
	return this->level;
}

void
Logger::SetLogName ( const std::string& name )
{
	this->name = name;
	this->hasname = name != "";
}

void
Logger::SetTimeStamping ( bool dotimestamp )
{
	this->timestamp = dotimestamp;
}

void
Logger::SetOutputter ( std::function<void
( const std::string& )> o )
{
	this->out = o;
}

inline void Logger::doFirst() {
	if (this->first) {
		if (this->timestamp) {
			Logger::getTime();
			oss << " : ";
		}
		if (this->hasname) {
			oss << this->name << ", ";
		}
		this->first = false;
	}
}

// UCLIBC "bugfix"
#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW 4
#endif

void Logger::getTime(void) {
	timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts)) {
		throw Utils::ErrnoException("Failed to get time");
	}
	oss.precision(6);
	oss << std::fixed << ts.tv_sec + (ts.tv_nsec / 1e9);
}

Logger& Logger::operator <<(const std::string& msg) {
	this->doFirst();
	oss << msg;
	return *this;
}

Logger& Logger::operator <<(const int msg) {
	this->doFirst();
	oss << msg;
	return *this;
}

Logger& Logger::operator <<(Manip& m) {
	m(*this);
	return *this;
}

Logger& Logger::flush() {
	if( this->lastlevel <= this->level )
	{
		oss << std::endl;
		this->out(oss.str());
	}
	oss.str("");
	this->first = true;
	return *this;
}

Logger::~Logger() {
	if( ! this->first )
	{
		this->flush();
	}
}


/*
 * Implementation of ClassWriter
 */

ClassWriter::ClassWriter(const char* name, bool life) :
		name(name), lifecycle(life) {
	if (this->lifecycle) {
		o("Constructed");
	}
}

inline void ClassWriter::o(const char* line) {
	std::cout << name << ": " << line << std::endl << std::flush;
}

ClassWriter::~ClassWriter() {
	if (this->lifecycle) {
		o("Destroyed");
	}
}

ClassWriter::ClassWriter() :
		name("unnamed"), lifecycle(false) {
}

/*
 * Scoped log implementation
 */

ScopedLog::ScopedLog(const std::string &name): name(name)
{
	logg << Logger::Debug << name << " start"<<lend;
}

ScopedLog::~ScopedLog()
{
	logg << Logger::Debug << name << " stop"<<lend;
}

Endl::~Endl(){}

Manip::~Manip(){}


}
