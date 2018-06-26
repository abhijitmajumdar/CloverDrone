#ifndef _INTERFACES_H
#define _INTERFACES_H

#include "rc_interface.h"
#include <cstdint>

typedef enum{
  RC=0,
  ROS,
  COMBINED
} Mode;

struct Commands {
	Commands() : throttle(0),roll(0),pitch(0),yaw(0),arming(false) {}
	float throttle;
	float roll;
	float pitch;
	float yaw;
  bool arming;
};

class Communication{
  public:
    Communication();
    ~Communication();
    void update(uint64_t);
    void print_commands();
    Commands commands;
  protected:
    Mode mode;
    uint64_t rc_update_time;
    float TH_MIN,TH_MAX;
    RadioControl *rc;
    Commands rc_commands;
};

#endif //_INTERFACES_H
