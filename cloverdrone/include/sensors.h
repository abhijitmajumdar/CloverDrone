#ifndef _SENSORS_H
#define _SENSORS_H

#include <iostream>
#include <RTIMULib.h>

namespace Sensor{
  void IMU_init(std::string*);
  void IMU_update(void);
  RTIMU_DATA IMU_data(void);
  void initialize(std::string*);
  void deinitialize(void);
}

#endif //_SENSORS_H
