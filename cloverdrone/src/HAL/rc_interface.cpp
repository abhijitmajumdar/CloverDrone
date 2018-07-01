#include "rc_interface.h"

RadioControl::RadioControl(std::string device_name="/dev/ttyUSB0") : request('#'),attempts(1)
{
	serial = new Serial(device_name,115200);
	load_defaults();
}

void RadioControl::load_defaults()
{
	for(int i=0;i<N_CHANNELS;i++) value[i] = value_default[i];
}

void RadioControl::update()
{
	serial->flush();
	serial->send(request);
	serial->receive(rcv_bytes,N_BYTES,attempts);
	std::string rcv_str = std::string((char*)rcv_bytes);
	size_t pos_start=0, pos_stop=0, idx=0;
	pos_start = rcv_str.find("!@");
	pos_stop = rcv_str.find("#");
	if ((pos_start!=std::string::npos) & (pos_stop!=std::string::npos) & (pos_start<pos_stop)){
		std::string oneline = rcv_str.substr(pos_start+2, pos_stop-pos_start-2);
		pos_start = -1;
		pos_stop = 0;
		while(idx<N_CHANNELS){
			pos_start = oneline.find(",",pos_start+1);
			if (pos_start==std::string::npos) break;
			rcv_int[idx++] = std::stoi(oneline.substr(pos_stop, pos_start-pos_stop));
			pos_stop = pos_start+1;
		}
		for(int i=0;i<N_CHANNELS;i++) value[i] = (rcv_int[i]>=500) ? ( (rcv_int[i]<=2500) ? (float(rcv_int[i])/1000) : value_default[i] ) : value_default[i];
	}
	else{
		load_defaults();
	}
}

void RadioControl::print_values()
{
	for(int _idx=0;_idx<N_CHANNELS;_idx++) std::cout<< value[_idx] << '\t';
	std::cout<<'\n';
}
