/*

	libutils - a collection of usable tools

	Copyright (c) 2023 Tor Krill <tor@openproducts.com>

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
	along with libutils.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SERIAL_H
#define SERIAL_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <termios.h>
#include <string>
#include <sys/ioctl.h>
#include <fstream>

#include <ext/stdio_filebuf.h>

namespace Utils {

class Serial {
public:
	Serial(const char* dev="/dev/ttyS0", speed_t speed=B115200);
	virtual ~Serial();

	void Flush();
	void Drain();
	void SetBaud(speed_t speed);
	int ReadControlSignals();
	void WriteControlSignals(int sig);

	ssize_t Write(const char* buf, ssize_t len);
	ssize_t Read(char* buf, ssize_t len);

	int GetFD(){ return port;}
	std::iostream& GetStream(){return *stream;}

	static speed_t ToSpeed(unsigned int spd);
	static unsigned int SpeedToD(speed_t spd);
private:
	std::string device;
	int port;
	__gnu_cxx::stdio_filebuf<char> *fb;
	std::iostream* stream;
	struct termios savetio;
	struct termios newtio;
};

}

#endif /* SERIAL_H_ */
