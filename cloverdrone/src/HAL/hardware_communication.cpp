#include "hardware_communication.h"
extern "C" {
#include <asm/termbits.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
}
#include <iostream>
#include <string.h>
#include <stdexcept>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

HardwareCommunication::HardwareCommunication(std::string device) : devicename(device),device_descriptor(-1)
{
  if (!device_open()) report_error("Could not open device "+devicename);
}

HardwareCommunication::~HardwareCommunication()
{
  device_close();
}

bool HardwareCommunication::device_open()
{
  device_descriptor  = open(this->devicename.c_str(),O_RDWR | O_NOCTTY);
  if (!is_open()) return false;
  return true;
}

void HardwareCommunication::device_close()
{
  if (is_open()) close(device_descriptor);
}

void HardwareCommunication::report_error(std::string err_str)
{
  throw std::runtime_error(err_str);
}

bool HardwareCommunication::write_seq(uint8_t * data, uint8_t len)
{
  if (!is_open()) return false;
  return (write(device_descriptor, data, len) == len);
}

bool HardwareCommunication::read_seq(uint8_t * data, uint8_t len)
{
  if (!is_open()) return false;
  return (read(device_descriptor, data, len) == len);
}




Serial::Serial(std::string device, unsigned int baud) : HardwareCommunication(device),baud(baud)
{
  configure();
}

bool Serial::configure()
{
  struct termios tio;
  struct termios2 tio2;
  tio.c_cflag =  CS8 | CLOCAL | CREAD;
  tio.c_oflag = 0;
  tio.c_lflag = 0;       //ICANON;
  tio.c_cc[VMIN]=0;
  tio.c_cc[VTIME]=1;     // time out every .1 sec
  ioctl(device_descriptor,TCSETS,&tio);
  ioctl(device_descriptor,TCGETS2,&tio2);
  tio2.c_cflag &= ~CBAUD;
  tio2.c_cflag |= BOTHER;
  tio2.c_ispeed = baud;
  tio2.c_ospeed = baud;
  ioctl(device_descriptor,TCSETS2,&tio2);
  ioctl(device_descriptor,TCFLSH,TCIOFLUSH);
  return true;
}

bool Serial::send(uint8_t data)
{
  return write_seq(&data, 1);
}

bool Serial::send(unsigned char* data, uint8_t len)
{
  return write_seq(data, len);
}

bool Serial::send(std::string data)
{
  uint8_t len = data.size(); //Do i need to add 1 here to factor in '\0'?
  char _data[len];
  strncpy(_data,data.c_str(),len);
  return write_seq((unsigned char*)_data, len);
}

bool Serial::receive(unsigned char* data, uint8_t len)
{
  return read_seq(data, len);
}

void Serial::flush(void)
{
	ioctl(device_descriptor,TCFLSH,TCIOFLUSH);
}




I2C::I2C(std::string device, int slave_address) : HardwareCommunication(device),slave_address(slave_address)
{
  if (!configure()) report_error("Could not connect to slave device");
}

bool I2C::configure()
{
  if (ioctl(device_descriptor, I2C_SLAVE, slave_address)<0) return false;
  return true;
}

bool I2C::send(uint8_t data, uint8_t reg_address)
{
  uint8_t _data[2];
  _data[0] = reg_address;
  _data[1] = data;
  return write_seq(_data, 2);
}

bool I2C::send(unsigned char* data, uint8_t len, uint8_t reg_address)
{
  uint8_t _data[len+1];
  _data[0] = reg_address;
  for(uint8_t _idx=0;_idx<len;_idx++) _data[_idx+1]=data[_idx];
  return write_seq(_data, len+1);
}

bool I2C::send(std::string data, uint8_t reg_address)
{
  uint8_t len = data.size(); //Do i need to add 1 here to factor in '\0'?
  char _data[len+1];
  _data[0] = reg_address;
  strncpy(&_data[1],data.c_str(),len);
  return write_seq((unsigned char*)_data, len+1);
}

bool I2C::receive(unsigned char* data, uint8_t len, uint8_t reg_address)
{
  if (!write_seq(&reg_address, 1)) return false;
  return read_seq(data, len);
}
