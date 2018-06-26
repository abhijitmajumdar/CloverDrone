#ifndef _HARDWARECOMMUNICATION_H
#define _HARDWARECOMMUNICATION_H

#include <iostream>
#include <cstdint>

class HardwareCommunication{
  public:
    HardwareCommunication(std::string device);
    ~HardwareCommunication();
    bool device_open();
    void device_close();
    bool read_seq(uint8_t *, uint8_t);
    bool write_seq(uint8_t * data, uint8_t len);
    bool is_open() {return device_descriptor>=0;}
    void report_error(std::string);
    int device_descriptor;
    std::string devicename;
  private:
};

class Serial : public HardwareCommunication{
  public:
    Serial(std::string, unsigned int);
    bool configure();
    bool send(uint8_t);
    bool send(unsigned char*, uint8_t);
    bool send(std::string);
    bool receive(unsigned char*, uint8_t);
    void flush(void);
  private:
    unsigned int baud;
};

class I2C : public HardwareCommunication{
  public:
    I2C(std::string, int);
    bool configure();
    bool send(uint8_t data, uint8_t reg_address);
    bool send(unsigned char* data, uint8_t len, uint8_t reg_address);
    bool send(std::string data, uint8_t reg_address);
    bool receive(unsigned char* data, uint8_t len, uint8_t reg_address);
  private:
    int slave_address;
};

#endif //_HARDWARECOMMUNICATION_H
