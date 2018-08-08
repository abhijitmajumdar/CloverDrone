#include "interfaces.h"
#include <iostream>
#include "configuration.h"
#include "utils.h"

Communication::Communication() : TH_MIN(1.0),TH_MAX(2.0),mode(Mode(qConstants.values["MODE"]))
{
  if (mode==COMBINED | mode==RC) rc = new RadioControl(qConstants.defines["RC_DEVICE"]);
}

Communication::~Communication()
{
  if (mode==COMBINED | mode==RC) delete rc;
}

void Communication::update(uint64_t system_time)
{
  if (((system_time-rc_update_time)>=qConstants.values["TIME_TO_GET_RCUSB"]) & (mode==RC))
  {
    rc->update();
    rc_commands.throttle = Utils::scale(&(rc->value[CH_THROTTLE]), &qConstants.values["CH_THROTTLE_MIN_CALIBRATE"], &qConstants.values["CH_THROTTLE_MAX_CALIBRATE"],&TH_MIN,&TH_MAX);
    rc_commands.roll = rc->value[CH_ROLL]-qConstants.values["CH_ROLL_CALIBRATE"];
    rc_commands.pitch = rc->value[CH_PITCH]-qConstants.values["CH_PITCH_CALIBRATE"];
    rc_commands.yaw = rc->value[CH_YAW]-qConstants.values["CH_YAW_CALIBRATE"];
    rc_commands.arming = (rc->value[CH_GEAR]>qConstants.values["CH_GEAR_CALIBRATE"])?true:false;
    rc_update_time = system_time;
  }
  if (mode==RC){
    commands.throttle = rc_commands.throttle;
    commands.roll = rc_commands.roll;
    commands.pitch = rc_commands.pitch;
    commands.yaw = rc_commands.yaw;
    commands.arming = rc_commands.arming;
  }
}

void Communication::print_commands()
{
  std::cout<< "Commands(Th,Roll,Pitch,Yaw,Arm): ";
  std::cout<< commands.throttle << '\t';
  std::cout<< commands.roll << '\t';
  std::cout<< commands.pitch << '\t';
  std::cout<< commands.yaw << '\t';
  std::cout<< commands.arming << '\n';
}
