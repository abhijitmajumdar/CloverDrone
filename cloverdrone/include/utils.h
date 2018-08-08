#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <vector>

#define USE_USLEEP

namespace Utils{
  void move_to_core(int);
  void ctrl_c_exit(void (*f)(int));
  void sleep(unsigned long);
  float scale(float*,float*,float*,float*,float*);
  std::string get_configfile_path();
  void throw_error(std::string);
  std::vector<std::string> split_str(std::string str, std::string delimiter);
  std::string last_in_str(std::string str, std::string delimiter);
}

#endif //_UTILS_H_
