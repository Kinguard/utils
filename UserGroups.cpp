/*
 * UserGroups.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: tor
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
	struct passwd* pwd;

	if(!(pwd=getpwnam(username.c_str()))){
		throw ErrnoException("Failed to get pw from user");
	}

	return pwd->pw_uid;
}

std::string
Utils::User::UIDToUser ( uid_t uid )
{
	ScopedLock l(umutex);
	struct passwd* pwd;

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
	gid_t *groups;
	struct passwd *pw;
	struct group *gr;

	groups =(gid_t *) malloc(ngroups * sizeof (gid_t));
	if (groups == NULL) {
		throw std::runtime_error("Unable to allocate memory");
	}

	pw = getpwnam(user.c_str());
	if (pw == NULL) {
		free(groups);
		throw ErrnoException("Failed to get pw from username");
	}

	if (getgrouplist(user.c_str(), pw->pw_gid, groups, &ngroups) == -1){
		free(groups);
		throw ErrnoException("Failed to get grouplist");
	}

	for (int j = 0; j < ngroups; j++) {
		gr = getgrgid(groups[j]);
		if (gr != NULL){
			res.push_back(pair<string,gid_t>(gr->gr_name,gr->gr_gid));
		}
	}
	free(groups);
	return res;
}

std::string
Utils::Group::GIDToGroup ( gid_t gid )
{
	ScopedLock l(gmutex);
	struct group* grp;

	if(!(grp=getgrgid(gid))){
		throw ErrnoException("Failed to get group id");
	}

	return grp->gr_name;
}

gid_t
Utils::Group::GroupToGID ( const std::string& groupname )
{
	ScopedLock l(gmutex);
	struct group* grp;

	if(!(grp=getgrnam(groupname.c_str()))){
		throw ErrnoException("Failed to get group name");
	}

	return grp->gr_gid;
}
