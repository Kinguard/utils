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

#include <grp.h>
#include <pwd.h>

#include "Mutex.h"
#include "Exceptions.h"

#include "UserGroups.h"

using namespace Utils;
using namespace std;

static Mutex umutex;
static Mutex gmutex;

uid_t
Utils::User::UserToUID ( const std::string& username )
{
	ScopedLock l(umutex);
	struct passwd* pwd = nullptr;

	if(!(pwd=getpwnam(username.c_str()))){
		throw ErrnoException("Failed to get pw from user");
	}

	return pwd->pw_uid;
}

std::string
Utils::User::UIDToUser ( uid_t uid )
{
	ScopedLock l(umutex);
	struct passwd* pwd = nullptr;

	if(!(pwd=getpwuid(uid))){
		throw ErrnoException("Failed to get pwd from uid");
	}

	return pwd->pw_name;
}

std::list<std::pair<std::string, gid_t> >
Utils::User::Groups ( const std::string& user )
{
	ScopedLock l(umutex);

	list<pair<string,gid_t> > res;
	int ngroups=20;
	gid_t *groups = nullptr;
	struct passwd *pw = nullptr;
	struct group *gr = nullptr;

	groups =(gid_t *) malloc(ngroups * sizeof (gid_t));
	if (groups == nullptr) {
		throw std::runtime_error("Unable to allocate memory");
	}

	pw = getpwnam(user.c_str());
	if (pw == nullptr) {
		free(groups);
		throw ErrnoException("Failed to get pw from username");
	}

	if (getgrouplist(user.c_str(), pw->pw_gid, groups, &ngroups) == -1){
		free(groups);
		throw ErrnoException("Failed to get grouplist");
	}

	for (int j = 0; j < ngroups; j++) {
		gr = getgrgid(groups[j]);
		if (gr != nullptr){
			res.emplace_back(gr->gr_name,gr->gr_gid);
		}
	}
	free(groups);
	return res;
}

std::string
Utils::Group::GIDToGroup ( gid_t gid )
{
	ScopedLock l(gmutex);
	struct group* grp = nullptr;

	if(!(grp=getgrgid(gid))){
		throw ErrnoException("Failed to get group id");
	}

	return grp->gr_name;
}

gid_t
Utils::Group::GroupToGID ( const std::string& groupname )
{
	ScopedLock l(gmutex);
	struct group* grp = nullptr;

	if(!(grp=getgrnam(groupname.c_str()))){
		throw ErrnoException("Failed to get group name");
	}

	return grp->gr_gid;
}
