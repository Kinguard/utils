/*
 * NetServer.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: tor
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
