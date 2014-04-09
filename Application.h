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
