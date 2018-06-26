#ifndef _INTERFACES_ROS
#define _INTERFACES_ROS

#include "interfaces.h"
#include "ros_interface.h"

class CommunicationROS : public Communication{
  public:
    CommunicationROS(int,char**);
    ~CommunicationROS();
    void update(uint64_t);
    bool check_ros();
  protected:
    uint64_t ros_update_time;
    ROSInterface *ros_interface;
    Commands ros_commands;
};

#endif //_INTERFACES_ROS
