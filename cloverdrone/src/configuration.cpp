#include "configuration.h"
#include "utils.h"

std::map<std::string,float> qConstants = {};

//Make sure to compile with "-std=c++11"
void Configuration::read_config_file(std::string config_filepath)
{
	std::fstream configfile;
	configfile.open(config_filepath+"/config.txt",std::fstream::out | std::fstream::in);
	if(configfile.is_open())
	{
		//Load the configurations
		std::string line;
		while(std::getline(configfile,line))
		{
			std::istringstream is_line(line);
			std::string key;
			if( getline(is_line, key, '=') )
			{
				std::string value;
				if(getline(is_line, value) )
				{
					qConstants[key] = stof(value);
				}
			}
		}
		configfile.close();
	}
	else
	{
		Utils::throw_error("Error reading config file!");
	}
	Configuration::check_necessary_constants();
	if(qConstants["DEBUG"]) Configuration::print_loaded_constants();
}

void Configuration::check_necessary_constants()
{
	for (std::list<std::string>::iterator it=necessary_constants.begin(); it!=necessary_constants.end(); it++)
	{
		if(!qConstants.count(*it)) Utils::throw_error("Necessay parameter \'"+*it+"\' not defined!");
	}
}

void Configuration::print_loaded_constants()
{
	std::cout<<"\n\nConfiguration parameters used:\n";
	for(std::map<std::string,float>::iterator it=qConstants.begin(); it!=qConstants.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	std::cout<<"\n";
}
