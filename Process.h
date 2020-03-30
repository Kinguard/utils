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

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <list>
#include <tuple>

using namespace std;

namespace Utils {

namespace Process {

	/**
	 * @brief Exec Execute command and wait for completion
	 * @param cmd command to execute
	 * @return Tuple bool status and string with output from stdout
	 */
	tuple<bool, string> Exec(const string& cmd);

	/**
	 * @brief Fork of a process and completely detach it from this process
	 *        Working directory is / and stdin, stdout and stderr redirected
	 *        to /dev/null (see more at man(3) daemon)
	 * @param cmd path and executable to execute
	 * @param args list of strings supplied to executable
	 *
	 */
	void Spawn(const string& cmd, const list<string>& args={});

}
}

#endif // PROCESS_H
