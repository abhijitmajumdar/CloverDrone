#include "ros_interface.h"

ROSInterface::ROSInterface(int _argc,char** _argv)
{
  ros::init(_argc, _argv, "cloverdrone_ros_node");
  n = new ros::NodeHandle();
  sub_target = n->subscribe("target", 10, &ROSInterface::target_update, this);
  //pub_status = n->advertise<cloverdrone_ros_msgs::DroneStatus>("status", 10);
  // Maybe use ros timers here (http://wiki.ros.org/roscpp/Overview/Timers)
  // or give an option to do so, like an argument
  load_defaults();
}

ROSInterface::~ROSInterface()
{
  delete n;
}

void ROSInterface::load_defaults()
{
  this->arming = false;
  this->throttle = 1.0;
  this->target_pose[0] = 0;
  this->target_pose[1] = 0;
  this->target_pose[2] = 0;
}

void ROSInterface::target_update(const cloverdrone_ros_msgs::DroneTarget::ConstPtr& msg)
{
  this->arming = msg->arming.data;
  this->throttle = msg->throttle.data;
  this->target_pose[0] = msg->pose.x;
  this->target_pose[1] = msg->pose.y;
  this->target_pose[2] = msg->pose.z;
}

void ROSInterface::update()
{
  ros::spinOnce();
}
void ROSInterface::print_values()
{
  std::cout<<"ROS commands(armed,th,roll,pitch,yaw): ";
  std::cout<< this->arming << '\t';
  std::cout<< this->throttle << '\t';
  std::cout<< this->target_pose[0] << '\t';
  std::cout<< this->target_pose[1] << '\t';
  std::cout<< this->target_pose[2] << '\t';
  std::cout<< '\n';
}
