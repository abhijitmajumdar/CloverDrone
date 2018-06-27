#ifndef _RCINTERFACE_H
#define _RCINTERFACE_H

#include <iostream>
#include "hardware_communication.h"

#define N_BYTES 27
#define FSR6B

/*
On the TGY-iA6 receiver
  CH1: Left-Right/Roll
  CH2: Up-Down/Pitch
  CH3: Throttle
  CH4: Yaw
  CH5: SWC
  CH6: SWB
*/
#ifdef TGYIA6
enum{
	CH_ROLL = 0,
	CH_PITCH,
	CH_THROTTLE,
  CH_YAW,
  CH_SWC,
  CH_SWB,
  N_CHANNELS
};
const float value_default[N_CHANNELS] = {1.5,1.5,1.0,1.5,1.0,1.0};
#endif

/*
On the FS-TH9X receiver
  CH1: Left-Right/Roll
  CH2: Up-Down/Pitch
  CH3: Throttle
  CH4: Yaw
  CH5: Gear
*/
#ifdef FSR6B
enum{
	CH_ROLL = 0,
	CH_PITCH,
	CH_THROTTLE,
  CH_YAW,
  CH_GEAR,
  N_CHANNELS
};

/*
On the FS-TH9X receiver
  CH1: Left-Right/Roll
  CH2: Up-Down/Pitch
  CH3: Throttle
  CH4: Yaw
  CH5: Gear
*/
#ifdef FSR9B
enum{
	CH_ROLL = 0,
	CH_PITCH,
	CH_THROTTLE,
  CH_YAW,
  CH_GEAR,
  N_CHANNELS
};
const float value_default[N_CHANNELS] = {1.468,1.480,1.0,1.478,1.0};
#endif

class RadioControl
{
  public:
    RadioControl(std::string);
    void update();
		void print_values();
    float value[N_CHANNELS];
  private:
		void load_defaults();
    Serial *serial;
    char request;
    unsigned char rcv_bytes[N_BYTES];
    int rcv_int[N_CHANNELS];
		unsigned int attempts,timeout;
};

#endif //_RCINTERFACE_H
