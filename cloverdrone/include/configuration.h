#ifndef _CONFIGRATION_H
#define _CONFIGRATION_H

#include <iostream>
#include <fstream> //configfile File handler
#include <sstream> //line String handler
#include <string>
#include <map>
#include <list>

/*
 * qConstants: contains a map of constants that can be refrenced using the constant name as strings
 * Example usage: qConstants["TIME_TO_COMPUTE"]
 * Can be used to read or write to the constant value
 * Use 1->True, 0->False for boolean vales
 * MODE: 0->RC(default), 1->ROS, 3->COMBINED
 */
extern std::map<std::string,float> qConstants;

/*
Make sure these parameters are defined in the config file
*/
static std::list<std::string> necessary_constants = {
	"TIME_TO_COMPUTE", "TIME_TO_UPDATEMOTOR", "TIME_TO_ARM", "I_THROTTLE_TRIGGER", "PD_THROTTLE_TRIGGER",
	"YAW_PA", "YAW_P", "YAW_I", "YAW_D", "ROLL_PA", "ROLL_P", "ROLL_I", "ROLL_D", "PITCH_PA", "PITCH_P", "PITCH_I", "PITCH_D",
	"CH_THROTTLE_MIN_CALIBRATE", "CH_THROTTLE_MAX_CALIBRATE",
	"CH_PITCH_CALIBRATE", "CH_ROLL_CALIBRATE", "CH_YAW_CALIBRATE", "CH_GEAR_CALIBRATE",
	"MODE"
};

namespace Configuration{
	void read_config_file(std::string);
	void print_loaded_constants();
	void check_necessary_constants();
}

#endif //_CONFIGRATION_H
