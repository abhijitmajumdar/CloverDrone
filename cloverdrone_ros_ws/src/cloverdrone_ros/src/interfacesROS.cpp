#include "interfacesROS.h"
#include "configuration.h"
#include "utils.h"

CommunicationROS::CommunicationROS(int _argc, char** _argv) : Communication()
{
  if (mode==COMBINED | mode==ROS) ros_interface = new ROSInterface(_argc,_argv);
}

CommunicationROS::~CommunicationROS()
{
  if (mode==COMBINED | mode==ROS) delete ros_interface;
}

bool CommunicationROS::check_ros()
{
  return (mode==COMBINED | mode==ROS)?ros::ok():true;
}

void CommunicationROS::update(uint64_t system_time)
{
  if (((system_time-rc_update_time)>=qConstants.values["TIME_TO_GET_RCUSB"]) & (mode==COMBINED | mode==RC))
  {
    rc->update();
    rc_commands.throttle = Utils::scale(&(rc->value[CH_THROTTLE]), &qConstants.values["CH_THROTTLE_MIN_CALIBRATE"], &qConstants.values["CH_THROTTLE_MAX_CALIBRATE"],&TH_MIN,&TH_MAX);
    rc_commands.roll = rc->value[CH_ROLL]-qConstants.values["CH_ROLL_CALIBRATE"];
    rc_commands.pitch = rc->value[CH_PITCH]-qConstants.values["CH_PITCH_CALIBRATE"];
    rc_commands.yaw = rc->value[CH_YAW]-qConstants.values["CH_YAW_CALIBRATE"];
    rc_commands.arming = (rc->value[CH_GEAR]>qConstants.values["CH_GEAR_CALIBRATE"])?true:false;
    rc_update_time = system_time;
  }
  if (((system_time-ros_update_time)>=qConstants.values["TIME_TO_ROS_SPIN"]) & (mode==COMBINED | mode==ROS))
  {
    ros_interface->update();
    ros_commands.throttle = ros_interface->throttle;
    ros_commands.roll = ros_interface->target_pose[0];
    ros_commands.pitch = ros_interface->target_pose[1];
    ros_commands.yaw = ros_interface->target_pose[2];
    ros_commands.arming = ros_interface->arming;
    ros_update_time = system_time;
  }

  if (mode==RC){
    commands.throttle = rc_commands.throttle;
    commands.roll = rc_commands.roll;
    commands.pitch = rc_commands.pitch;
    commands.yaw = rc_commands.yaw;
    commands.arming = rc_commands.arming;
  }
  else if (mode==ROS) {
    commands.throttle = rc_commands.throttle;
    commands.roll = rc_commands.roll;
    commands.pitch = rc_commands.pitch;
    commands.yaw = rc_commands.yaw;
    commands.arming = rc_commands.arming;
  }
  else if(mode==COMBINED){
    commands.arming = rc_commands.arming | ros_commands.arming;
    commands.throttle = commands.arming?rc_commands.throttle:TH_MIN;
    commands.roll = rc_commands.arming?rc_commands.roll:ros_commands.roll;
    commands.pitch = rc_commands.arming?rc_commands.pitch:ros_commands.pitch;
    commands.yaw = rc_commands.arming?rc_commands.yaw:ros_commands.yaw;
  }
}
