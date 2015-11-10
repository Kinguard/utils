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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>
#include <memory>
#include <map>

#include "ArgParser.h"
#include "ClassTools.h"
#include "Logger.h"

#include <signal.h>

using namespace std;

namespace Utils {

class SigHandler : public Utils::NoCopy
{
	SigHandler()
	{
	}

	static void sighandler(int signo)
	{
		SigHandler::Instance().HandleSignal(signo);
	}

	void HandleSignal(int signo)
	{
		if( handlers.find(signo) != handlers.end() )
		{
			handlers[signo](signo);
		}
	}

public:

	static SigHandler& Instance()
	{
		static SigHandler signal;

		return signal;
	}

	void AddHandler(int signo, std::function<void (int)> fn )
	{
		struct sigaction action;
		action.sa_handler = SigHandler::sighandler;
		action.sa_flags = 0;
		sigemptyset(&action.sa_mask);

		handlers[signo] = fn;
		sigaction(signo, &action, nullptr);
	}

	void RemoveHandler(int signo)
	{
		struct sigaction action;
		action.sa_handler = SIG_DFL;
		action.sa_flags = 0;
		sigemptyset(&action.sa_mask);

		sigaction(signo, &action, nullptr);

		if( handlers.find(signo) != handlers.end() )
		{
			handlers.erase( signo );
		}
	}

	virtual ~SigHandler()
	{

	}
private:
	map<int, std::function<void (int) > > handlers;
};

class Application : public Utils::NoCopy
{
public:
	Application(const string& name);

	int Start(int argc, char** argv);

	virtual void Startup() {}
	virtual void Main() {}
	virtual void ShutDown() {}

	void SetExitcode(int code);
	virtual ~Application();
protected:

	/* Hooks */
	virtual void PreStartup(){}
	virtual void PostStartup(){}

	virtual void PreMain(){}
	virtual void PostMain(){}

	virtual void PreShutdown(){}
	virtual void PostShutdown(){}

	/* Attributes */
	string name;
	ArgParser options;
	int exitcode;
};

typedef std::shared_ptr<Application> ApplicationPtr;

class DaemonApplication : public Application
{
public:
	DaemonApplication(const string& name, const string& pidpath, const string& user, const string& group);

	virtual ~DaemonApplication();
protected:
	virtual void PostStartup();
	virtual void PreMain();
private:
	string pidpath;
	string runasuser;
	string runasgroup;
};

typedef std::shared_ptr<DaemonApplication> DaemonApplicationPtr;

}

#endif // APPLICATION_H
