#ifndef _GPIO_ACCESS_H
#define _GPIO_ACCESS_H

#include <iostream>

/*
Pin numbers are BCM numbering in the domain {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25}
*/

typedef enum {
  IN=0,
  OUT
} pin_direction;

typedef enum {
  LOW=0,
  HIGH,
  NA
} pin_state;

#define PATH_MAX 35
#define BUFFER_MAX 4
static const char s_directions_str[]  = "in\0out";
static const char s_values_str[] = "01";

class GPIO{
  public:
    GPIO(int,pin_direction);
    ~GPIO();
    bool set(bool);
    bool set(pin_state);
  private:
    void report_error(std::string);
    int pin_number;
    pin_state state;
    int fd;
    char buffer[BUFFER_MAX];
    char path_export[PATH_MAX];
    char path_unexport[PATH_MAX];
    char path_direction[PATH_MAX];
    char path_value[PATH_MAX];
    int bytes_written;
    int timeout;
};

#endif //_GPIO_ACCESS_H
