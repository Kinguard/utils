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

#ifndef USERGROUPS_H_
#define USERGROUPS_H_

#include <grp.h>

#include <list>
#include <utility>
#include <string>


namespace Utils
{

namespace User
{
	uid_t UserToUID(const std::string& username);
	std::string UIDToUser(uid_t uid);
	std::list<std::pair<std::string,gid_t> > Groups(const std::string& user);
}

namespace Group
{
	std::string GIDToGroup(gid_t gid);
	gid_t GroupToGID(const std::string& groupname);
}

}



#endif /* USERGROUPS_H_ */
