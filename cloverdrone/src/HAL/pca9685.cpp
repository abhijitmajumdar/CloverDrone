#include "pca9685.h"
#include <unistd.h>

PCA9685::PCA9685(std::string device, int address, int frequency) : I2C(device, address),freq(frequency)
{
  usleep(10000); //Give time to properly connect to pca9685, via I2C constructor
  // Enable auto increment
  uint8_t auto_increment = 0x20;
  send(auto_increment, PCA9685_MODE1);
  set_frequency(frequency);
  reset();
}

void PCA9685::set_frequency(int frequency)
{
  //40<frequency<1000
  frequency = (frequency > 1000 ? 1000 : (frequency < 40 ? 40 : frequency));
  uint8_t prescale = (int)(25000000.0f / (4096 * frequency) - 0.5f);
  receive(_buffer, 1, PCA9685_MODE1);
  uint8_t sleep = (_buffer[0] & 0x7F) | 0x10;
  uint8_t wake = (_buffer[0] & 0x7F) & 0xEF;
  uint8_t restart = (_buffer[0] & 0x7F) | 0x80;
  send(sleep, PCA9685_MODE1);
  send(prescale, PCA9685_PRESCALE);
  send(wake, PCA9685_MODE1);
  usleep(10000); // Some time for clock to initialize
  send(restart, PCA9685_MODE1);
  usleep(10000); // Some time after a reset
  this->freq = frequency;
  this->cycle_time_ms = 1000.0f/frequency;
}

void PCA9685::reset()
{
  _buffer[0] = 0x00;
  _buffer[1] = 0x00;
  _buffer[2] = 0x10;
  _buffer[3] = 0x00;
  send(_buffer,4,PCA9685_LEDALL_ON_L);
}

// pin 0-15
void PCA9685::set_pwm(int pin, int on, int off)
{
  uint8_t reg = pin_base_register(pin);
  on = on & 0x0FFF;
  off = off & 0x0FFF;
  _buffer[0] = uint8_t(on%256);
  _buffer[1] = uint8_t(on/256);
  _buffer[2] = uint8_t(off%256);
  _buffer[3] = uint8_t(off/256);
  send(_buffer,4,reg);
}

uint8_t PCA9685::pin_base_register(int pin)
{
  return PCA9685_LED0_ON_L + (4 * pin);
}

/*
Servo signal drivers
*/
int PCA9685::calc_ticks(float* millis)
{
	return (int)(PCA9685_MAX_PWM*(*millis)/cycle_time_ms +0.5f);
}

//pin 0-15
void PCA9685::set_servo_signal(int pin, float* millis)
{
	set_pwm(pin, 0, calc_ticks(millis));
}
