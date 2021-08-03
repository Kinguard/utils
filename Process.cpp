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

#include <sstream>
#include <csignal>
#include <vector>
#include <unistd.h>

#include "Exceptions.h"

#include "Process.h"

// We really need SIGCHLD for pclose to be able to fetch
// exitcodes etc.
// We check and temporarily enable signal if ignored


/**
 * @brief check_signal
 * @param act sigaction to operate on.
 * @return tuple blocked or not, operation status, status message
 */
static tuple<bool, bool, string> check_signal(struct sigaction *act)
{
	bool temp_unblocked = false;
	if( sigaction(SIGCHLD, nullptr, act) < 0 )
	{
		return make_tuple(false, false,"Failed to fetch signal status");
	}

	if( ! (act->sa_flags & SA_SIGINFO) )
	{
		if( act->sa_handler == SIG_IGN )
		{
			temp_unblocked = true;
			act->sa_handler = SIG_DFL;

			if( sigaction(SIGCHLD, act, nullptr) < 0 )
			{
				return make_tuple(false, false, "Failed to alter signal status");
			}
		}
	}
	return make_tuple(temp_unblocked, true, "OK");
}

static void reset_signal(struct sigaction *act)
{
	if( ! (act->sa_flags & SA_SIGINFO) )
	{
		if( act->sa_handler == SIG_DFL)
		{
			act->sa_handler = SIG_IGN;

			if( sigaction(SIGCHLD, act, nullptr) < 0 )
			{
				// Not much todo :(
			}
		}
	}
}


tuple<bool, string> Utils::Process::Exec(const string &cmd)
{
	bool temp_unblocked = false, sig_result = false;
	string res_desc;
	struct sigaction act{};
	tie(temp_unblocked, sig_result, res_desc) = check_signal(&act);

	if( !sig_result )
	{
		return make_tuple(sig_result, res_desc);
	}

	FILE* pipe = popen(cmd.c_str(), "re");

	if (!pipe)
	{
		return make_tuple(false,"Failed popen");
	}

	char buffer[128];
	stringstream result;

	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != nullptr)
		{
			result << buffer;
		}
	}
	int res = pclose(pipe);

	bool ret = !((res < 0 ) ||  WEXITSTATUS(res) != 0);

	if( temp_unblocked )
	{
		reset_signal(&act);
	}

	return make_tuple( ret, result.str());
}

void Utils::Process::Spawn(const string &cmd, const list<string> &args)
{
	pid_t pid = fork();

	if( pid < 0 )
	{
		throw ErrnoException("Spawn: Failed to fork");
	}

	if( pid > 0 )
	{
		return;
	}

	if( daemon( 0, 0) < 0 )
	{
		exit(1);
	}

	vector<char*> cargs;

	//TODO: Figure out nice way to not leak memory via strdup
	cargs.push_back(strdup(cmd.c_str()));

	for(const auto& arg: args)
	{
		cargs.push_back(strdup(arg.c_str()));
	}

	cargs.push_back(nullptr);

	if( ::execvp( cmd.c_str(), &cargs[0]) == -1 )
	{
		perror("Spawn: execvp failed");
		exit(1);
	}

	// Should never get here
}
