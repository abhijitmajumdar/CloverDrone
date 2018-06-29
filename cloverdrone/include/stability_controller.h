#ifndef _STABILITY_CONTROLLER_H_
#define _STABILITY_CONTROLLER_H_

#include <thread>
#include <iostream>
#include <mutex>
#include "RTIMULib.h"
#include "actuators.h"
#include "sensors.h"
#include "controller.h"
#include "configuration.h"

class Stabilize
{
  public:
		Stabilize(int, std::string);
    ~Stabilize();
    bool running() { return this->is_running; }
    void stop();
    void arm(bool);
    uint64_t get_time() { return this->system_time; }
    bool check_time(uint64_t t, uint64_t ut) { return (this->system_time - t)>=ut; }
    void load_parameters();
    void set_targets(float,float,float,float);
    void get_accelerometer(float*,float*,float*);
    void get_gyroscope(float*,float*,float*);
    void get_magnetometer(float*,float*,float*);
    void get_pose(float*,float*,float*);
    void get_motor(float*,float*,float*,float*);
    void print_accelerometer();
    void print_gyroscope();
    void print_magnetometer();
    void print_pose();
    void print_motor();
  private:
    void stabilizer_routine(int);
    void safety_check();
    std::thread stabilizer_thread;
    std::mutex time_mutex;
    // Flow control
    bool is_running, armed;
    uint64_t system_time;
    uint64_t sense_time, compute_time, motor_time, arming_time;
    uint64_t sense_time_limit, compute_time_limit, motor_time_limit, arming_time_limit;
    // Control and Sensing variables
    qPIDvariables vPhi,vTheta,vGamma; // vPhi->Roll, vTheta->Pitch, vGamma->Yaw
    qMotorThrust vMotor;
    qTarget vTarget;
    RTIMU_DATA imud;
    qControl* quadController;
};

#endif //_STABILITY_CONTROLLER_H_
