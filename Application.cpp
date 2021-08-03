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

#include "Application.h"

#include "UserGroups.h"

#include <iostream>
#include <fstream>
#include <utility>

#include <sys/types.h>
#include <unistd.h>

namespace Utils
{

/*
 *    Application implementation
 */

Application::Application(const string &name): name(name), exitcode(0)
{
	logg.SetLogName(name);
}

int Application::Start(int argc, char** argv)
{

	this->PreStartup();
	this->Startup();
	this->PostStartup();

	this->options.AddOption( Option( '?', "help", Option::ArgNone, "0", "This helptext"));

	if( ! this->options.Parse( argc,argv) )
	{
		logg << Logger::Error << "Invalid arguments"<<lend;
		this->SetExitcode( 1 );
	}
	else
	{
		if ( this->options["help"] == "1" )
		{
			cerr << this->options.GetHelptext();
		}
		else
		{
			this->PreMain();
			this->Main();
			this->PostMain();
		}
	}

	this->PreShutdown();
	this->ShutDown();
	this->PostShutdown();

	return this->exitcode;
}

void Application::SetExitcode(int code)
{
	this->exitcode = code;
}

Application::~Application() = default;

/*
 * Daemon application implementation
 */

DaemonApplication::DaemonApplication(const string &name, string pidpath, string user, string group):
	Application(name), pidpath(std::move(pidpath)), runasuser(std::move(user)), runasgroup(std::move(group))
{

}

void DaemonApplication::PostStartup()
{
	// Add default daemon options
	this->options.AddOptions( {
				Option('d',"daemonize",Option::ArgNone,"0","Daemonize application"),
				Option('p',"pidfile", Option::ArgRequired, this->pidpath + "/" + name + ".pid", "Path to pidfile"),
				Option('u',"user", Option::ArgRequired, this->runasuser, "User to run as"),
				Option('g',"group", Option::ArgRequired, this->runasgroup, "Group to run as"),
				});
}

void DaemonApplication::PreMain()
{
	if( this->options["daemonize"] == "1" )
	{

		// Daemonize
		if( daemon( 0, 0) < 0 )
		{
			logg << Logger::Error << "Failed to daemonize"<<lend;
			throw ErrnoException("Unable to daemonize");
		}
		logg << Logger::Info << "Daemonized"<<lend;

		// Write pidfile
		ofstream of(this->options["pidfile"]);
		if( of.fail() )
		{
			throw ErrnoException("Failed open pidfile for write");
		}
		of<<getpid()<<endl;
		of.close();
	}

	// Possibly changing uid/gid

	if( setgid( Group::GroupToGID( this->options["group"] ) )  < 0 )
	{
		throw ErrnoException("Failed to set group:" + this->options["group"] );
	}

	if( setuid( User::UserToUID( this->options["user"] ) ) < 0 )
	{
		throw ErrnoException("Failed to setuid:" + this->options["user"]);
	}

}


DaemonApplication::~DaemonApplication() = default;

}
