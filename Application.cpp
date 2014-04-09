#include "Application.h"

#include "UserGroups.h"

#include <iostream>
#include <fstream>

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

Application::~Application()
{

}

/*
 * Daemon application implementation
 */

DaemonApplication::DaemonApplication(const string &name, const string &pidpath, const string &user, const string &group):
	Application(name), pidpath(pidpath), runasuser(user), runasgroup(group)
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
	// Possibly changing uid/gid

	if( setuid( User::UserToUID( this->options["user"] ) ) < 0 )
	{
		throw ErrnoException("Failed to setuid:" + this->options["user"]);
	}

	if( setgid( Group::GroupToGID( this->options["group"] ) )  < 0 )
	{
		throw ErrnoException("Failed to set group:" + this->options["group"] );
	}

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
}


DaemonApplication::~DaemonApplication()
{

}

}
