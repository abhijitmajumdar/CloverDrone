#ifndef _ACTUATORS_H
#define _ACTUATORS_H

#include <iostream>

namespace Actuator{
  void initialize(std::string device, int, int);
  void motors_engage(bool enable);
  void status_led(bool enable);
  void set_motors(float m1, float m2, float m3, float m4);
  void deinitialize(void);
}

#endif //_ACTUATORS_H
