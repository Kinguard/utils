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


}
