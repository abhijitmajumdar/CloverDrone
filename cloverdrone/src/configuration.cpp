#include "configuration.h"
#include "utils.h"

qC qConstants;

bool is_number(std::string *s)
{
	uint8_t len = s->length();
	while(len--){
		if( !((('0'<=(*s)[len])&((*s)[len]<='9')) | ((*s)[len]=='.') | ((*s)[len]=='-')) ){
			return false;
		}
	}
	return true;
}

bool is_boolean(std::string *s)
{
	return ( (s->compare("True")==0) | (s->compare("true")==0) | (s->compare("False")==0) | (s->compare("false")==0) );
}

bool stob(std::string *s)
{
	return ((s->compare("True")==0) | (s->compare("true")==0))?true:false;
}

//Make sure to compile with "-std=c++11"
void Configuration::read_config_file(std::string config_filepath)
{
	std::fstream configfile;
	configfile.open(config_filepath,std::fstream::out | std::fstream::in);
	if(!configfile.is_open()) Utils::throw_error("Error reading config file!");
	std::string line;
	char key[128],val[128];
	while(std::getline(configfile,line))
	{
		if ((line[0]=='#') || (line[0]=='\n'))
      continue;
    if (std::sscanf(line.insert(0,1,' ').c_str(),"%*[^0-9a-zA-Z]%[^=]=%s",key,val) != 2)
			continue;
		std::sscanf(key,"%s",key);
		std::string skey(key);
		std::string sval(val);
		if (is_number(&sval)){
			qConstants.values[key] = std::stof(sval);
		}
		else if (is_boolean(&sval)){
			qConstants.flags[key] = stob(&sval);
		}
		else{
			qConstants.defines[key] = sval;
		}
	}
	Configuration::check_necessary_constants();
	if(qConstants.flags["DEBUG"]) Configuration::print_loaded_constants();
}

void Configuration::check_necessary_constants()
{
	for (std::list<std::string>::iterator it=necessary_constants.begin(); it!=necessary_constants.end(); it++)
	{
		if(!(qConstants.values.count(*it) | qConstants.flags.count(*it) | qConstants.defines.count(*it))) Utils::throw_error("Necessay parameter \'"+*it+"\' not defined!");
	}
}

void Configuration::print_loaded_constants()
{
	std::cout<<"\n\nConfiguration parameters used:\n";
	for(std::map<std::string,float>::iterator it=qConstants.values.begin(); it!=qConstants.values.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	for(std::map<std::string,bool>::iterator it=qConstants.flags.begin(); it!=qConstants.flags.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	for(std::map<std::string,std::string>::iterator it=qConstants.defines.begin(); it!=qConstants.defines.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	std::cout<<"\n";
}
