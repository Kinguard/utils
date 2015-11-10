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

#include "NetServer.h"
#include "Logger.h"

namespace Utils
{
namespace Net
{

NetServer::NetServer (ServerSocketPtr serv, int timeout):
		shutdown(false), pendingreq(0), serv(serv)
{
	this->serv->SetTimeout(timeout, 0);
}


void
NetServer::Dispatch ( SocketPtr con )
{
	this->decreq();
}

void
NetServer::Run ()
{
	logg << Logger::Debug << "Starting up" << lend;
	do
	{
		SocketPtr con = this->serv->Accept();

		if( con )
		{
			this->increq();
			this->Dispatch(con);
		}
		else
		{
			if( this->serv->TimedOut() )
			{
				ScopedLock s(this->m );

				if( this->pendingreq == 0 )
				{
					this->shutdown = true;
					logg << Logger::Notice << "Server timed out. Terminating"<<lend;
				}
				else
				{
					logg << Logger::Notice << "Server timed out,"
							<<this->pendingreq
							<< " requests left. Not terminating"<<lend;
				}
			}
			else
			{
				//Todo: Error handling perhaps
				logg << Logger::Error << "Accept failed"<<lend;
			}

		}

	}while(!this->shutdown);
	logg << Logger::Debug << "Terminating" << lend;
}

void
NetServer::ShutDown ()
{
	this->shutdown = true;
}

NetServer::~NetServer ()
{
	// TODO Auto-generated destructor stub
}

void
NetServer::decreq ()
{
	ScopedLock s(this->m);

	if( this->pendingreq > 0)
	{
		this->pendingreq--;
	}
}

void
NetServer::increq ()
{
	ScopedLock s(this->m);

	this->pendingreq++;
}

} /* namespace Net */
} /* namespace Utils */
