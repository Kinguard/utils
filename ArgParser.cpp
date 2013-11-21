#include "ArgParser.h"

#include <iostream>

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
			opt.long_desc.c_str(),
			opt.type,
			0,
			0
		}
		);
	this->opts.push_back(opt);

	// Add to config
	this->insert(pair<string,string>(opt.long_desc, opt.default_val));
}

void
Utils::ArgParser::AddOptions( list<Option> l)
{
	for( auto opt: l)
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
		return false;
	}

	// Make sure last element in longopts is 0
	struct option last = this->longopts.back();
	if( last.name != 0 || last.has_arg != 0 || last.flag != 0 || last.val != 0 )
	{
		this->longopts.push_back( { 0, 0, 0, 0} );
	}

	int c;
	int opt_ind;
	while(1)
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
			this->reminder.push_back(argv[optind++]);
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


Utils::ArgParser::~ArgParser()
{

}

inline int
Utils::ArgParser::FindIndex(char c)
{
	int i = 0;
	for( auto x: this->opts)
	{
		if( x.short_desc == c )
		{
			return i;
		}
		i++;
	}
	return -1;
}
