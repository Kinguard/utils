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

#ifndef NETSERVER_H_
#define NETSERVER_H_

#include "ClassTools.h"
#include "Socket.h"
#include "Mutex.h"

namespace Utils
{
namespace Net
{

class NetServer: public NoCopy
{
public:
	NetServer (ServerSocketPtr serv, int timeout);

	virtual void Dispatch(SocketPtr con);
	void Run();
	void ShutDown();

	virtual	~NetServer ();
protected:
	void decreq();

private:
	bool shutdown;
	int pendingreq;
	Mutex m;
	ServerSocketPtr serv;
	void increq();
};

} /* namespace Net */
} /* namespace Utils */
#endif /* NETSERVER_H_ */
