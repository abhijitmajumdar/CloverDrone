#include "gpio_access.h"
extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
}
#include <stdexcept>

GPIO::GPIO(int pin, pin_direction dir) : pin_number(pin),state(NA),timeout(10)
{
  sprintf(path_export, "/sys/class/gpio/export");
  sprintf(path_unexport, "/sys/class/gpio/unexport");
  snprintf(path_direction, PATH_MAX, "/sys/class/gpio/gpio%d/direction", pin_number);
  snprintf(path_value, PATH_MAX, "/sys/class/gpio/gpio%d/value", pin_number);
  bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin_number);
  // Export pin
  fd = open(path_export, O_WRONLY);
  if (fd==-1) report_error("Could not export pin");
  write(fd, buffer, bytes_written);
  close(fd);
  usleep(500000);
  // Set direction of pin
  fd = open(path_direction, O_WRONLY);
  if (fd==-1) report_error("Could not open direction access to pin");
  write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3);
  close(fd);
  usleep(500000);
}

GPIO::~GPIO()
{
  fd = open(path_unexport, O_WRONLY);
  if (fd==-1) return;
  write(fd, buffer, bytes_written);
  close(fd);
}

bool GPIO::set(bool value)
{
  int _timeout = timeout;
  do{
    if(set(value?HIGH:LOW)) break;
    std::cout<<"\nWarning: retrying pin value access\n";
    usleep(1000);
  } while(_timeout--);
  return _timeout>0;
}

bool GPIO::set(pin_state value)
{
  if (value==state) return true;
  fd = open(path_value, O_WRONLY);
  if (fd==-1) return false;
  write(fd, &s_values_str[LOW == value ? 0 : 1], 1);
  close(fd);
  state = value;
  return true;
}

void GPIO::report_error(std::string err_str)
{
  throw std::runtime_error(err_str);
}
