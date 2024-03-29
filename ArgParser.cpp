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

#include "ArgParser.h"

#include <iostream>
#include <sstream>

#include <cstring>

Utils::ArgParser::ArgParser():map<string,string>()
{

}

void
Utils::ArgParser::AddOption(const Option& opt)
{
	// Append to short args
	this->shortargs.push_back(opt.short_desc);

	if( opt.type == Option::ArgOptional )
	{
	 this->shortargs.push_back(':');
	 this->shortargs.push_back(':');
	}

	if( opt.type == Option::ArgRequired )
	{
	 this->shortargs.push_back(':');
	}

	// Create long args version
	this->longopts.push_back(
		{
			strdup( opt.long_desc.c_str() ),
			opt.type,
			nullptr,
			0
		}
		);
	this->opts.push_back(opt);

	// Add to config
	this->insert(pair<string,string>(opt.long_desc, opt.default_val));
}

void
Utils::ArgParser::AddOptions( const list<Option>& l)
{
	for( const auto& opt: l)
	{
		this->AddOption(opt);
	}
}

bool
Utils::ArgParser::Parse(int argc, char ** const argv)
{
	bool ret = true;
	if( this->longopts.size() == 0 )
	{
		// Noting to do but this is no error
		return true;
	}

	// Make sure last element in longopts is 0
	struct option last = this->longopts.back();
	if( last.name != nullptr || last.has_arg != 0 || last.flag != nullptr || last.val != 0 )
	{
		this->longopts.push_back( { nullptr, 0, nullptr, 0} );
	}

	int c = 0;
	int opt_ind = 0;
	while(true)
	{
		opt_ind = 0;
		c = getopt_long(argc, argv, shortargs.c_str(), &this->longopts[0], &opt_ind);
		if( c == -1 )
		{
			break;
		}

		switch(c)
		{
		case 0:
			if( ( this->opts[opt_ind].type >  0 ) && optarg )
			{
				this->operator[](this->longopts[opt_ind].name) = optarg;
			}
			else
			{
				this->operator[](this->longopts[opt_ind].name) = "1";
			}
			break;
		case '?':
		case ':':
			ret = false;
			break;
		default:
			{
				int i=this->FindIndex(c);
				if( ( this->opts[i].type >  0 ) && optarg )
				{
					this->operator[](this->longopts[i].name) = optarg;
				}
				else
				{
					this->operator[](this->longopts[i].name) = "1";
				}
			}
		}
	}

	this->reminder.clear();

	if (optind < argc)
	{
		while (optind < argc)
		{
			this->reminder.emplace_back(argv[optind++]);
		}
	}

	return ret;
}

bool
Utils::ArgParser::HasReminder()
{
	return !this->reminder.empty();
}

list<string>
Utils::ArgParser::GetReminder()
{
	return this->reminder;
}

string Utils::ArgParser::GetHelptext()
{
	stringstream ss;
	ss << "Options:"<< endl;
	for( const Option& opt: this->opts )
	{
		ss << "  -" << opt.short_desc << "  --" << opt.long_desc << "\t"<< "["<< opt.default_val << "]\t: " << opt.helptext<< endl;
	}

	return ss.str();
}

void Utils::ArgParser::_Dump()
{

	for( const Option& opt: this->opts )
	{
		cout << "Options: ["<< opt.long_desc<< "] [" << opt.short_desc << "] [" << opt.default_val << "] " << opt.helptext << endl;
	}

	cout << "Short args: "<<this->shortargs << endl;

	for( const struct option& opt: this->longopts)
	{
		cout << "Longopt: ["<< opt.name << "] ["<<opt.has_arg<<"]"<<endl;
	}

}

Utils::ArgParser::~ArgParser()
{
	for( const option opt: this->longopts )
	{
		if( opt.name != nullptr )
		{
			free( (void*)opt.name );
		}
	}
}

inline int
Utils::ArgParser::FindIndex(char c)
{
	int i = 0;
	for( const auto& x: this->opts)
	{
		if( x.short_desc == c )
		{
			return i;
		}
		i++;
	}
	return -1;
}
