#ifndef _ROS_INTERFACE_H
#define _ROS_INTERFACE_H

#include <iostream>
// ROS based includes
#include "ros/ros.h"
#include "cloverdrone_ros_msgs/DroneTarget.h"
#include "cloverdrone_ros_msgs/DroneStatus.h"

class ROSInterface{
  public:
    ROSInterface(int,char**);
    ~ROSInterface();
    void update();
    //void send_status();
		void print_values();
    bool arming;
    float throttle;
    float target_pose[3];
  private:
    //float imu_data[9];
    ros::NodeHandle *n;
    ros::Subscriber sub_target;
    //ros::Publisher pub_status;
    void target_update(const cloverdrone_ros_msgs::DroneTarget::ConstPtr&);
};

#endif //_ROS_INTERFACE_H
