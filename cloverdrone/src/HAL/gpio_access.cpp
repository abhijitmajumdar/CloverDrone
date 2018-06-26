#include "gpio_access.h"
extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
}
#include <stdexcept>

#define DIRECTION_MAX 35
#define BUFFER_MAX 3
#define VALUE_MAX 30

GPIO::GPIO(int pin, pin_direction dir) : pin_number(pin),state(NA)
{
  int bytes_written;
  char buffer[BUFFER_MAX];
  char path[DIRECTION_MAX];
  bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin_number);
  snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin_number);
  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd==-1) report_error("Could not open access to pin");
  write(fd, buffer, bytes_written);
  close(fd);
  usleep(100000);
  fd = open(path, O_WRONLY);
  if (fd==-1) report_error("Could not open access to pin");
  write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3);
  close(fd);
}

GPIO::~GPIO()
{
  int bytes_written;
  char buffer[BUFFER_MAX];
  bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin_number);
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd==-1) return;
  write(fd, buffer, bytes_written);
  close(fd);
}

bool GPIO::set(bool value)
{
  return set(value?HIGH:LOW);
}

bool GPIO::set(pin_state value)
{
  if (value==state) return true;
  char path[VALUE_MAX];
  snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin_number);
  fd = open(path, O_WRONLY);
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
