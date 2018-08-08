#ifndef _ACTUATORS_H
#define _ACTUATORS_H

#include <iostream>

// BCM on RPi2
// A good reference is https://pinout.xyz
#define AMBER 24
#define BLUE 25
#define PWM_EN 27

namespace Actuator{
  void initialize(std::string device);
  void motors_engage(bool enable);
  void status_led(bool enable);
  void set_motors(float m1, float m2, float m3, float m4);
  void deinitialize(void);
}

#endif //_ACTUATORS_H
