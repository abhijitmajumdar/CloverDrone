#ifndef _PCA9685_H
#define _PCA9685_H

#include <iostream>
#include <cstdint>
#include "hardware_communication.h"

#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x06
#define PCA9685_LEDALL_ON_L 0xFA
#define PCA9685_MAX_PWM 4096

class PCA9685 : public I2C{
  public:
    PCA9685(std::string,int,int);
    void set_frequency(int frequency);
    void reset();
    void set_pwm(int pin, int on, int off);
    int calc_ticks(float* millis);
    void set_servo_signal(int pin, float* millis);
  private:
    uint8_t pin_base_register(int pin);
    uint8_t _buffer[100];
    int freq;
    float cycle_time_ms;
};

#endif //_PCA9685_H
