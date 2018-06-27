#include "rc_interface.h"

RadioControl::RadioControl(std::string device_name="/dev/ttyUSB0") : request('#'),timeout(10)
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
	do{
		serial->flush();
		serial->send(request);
		serial->send(request);
		attempts += 1;
		if (attempts>timeout) break;
	} while(!serial->receive(rcv_bytes,N_BYTES));
	if (attempts>timeout)
	{
		load_defaults();
	}
	else
	{
		size_t pos=0, ppos=0, idx=0;
		std::string rcv_str = std::string((char*)rcv_bytes);
		pos = rcv_str.find("\n");
		if (pos!=std::string::npos){
			std::string oneline = rcv_str.substr(0, pos);
			pos = -1;
			while(true & (idx<N_CHANNELS)){
				pos = rcv_str.find("\t",pos+1);
				if (pos==std::string::npos) break;
				rcv_int[idx++] = std::stoi(rcv_str.substr(ppos, pos));
				ppos = pos;
			}
		}
		for(int i=0;i<N_CHANNELS;i++) value[i] = (rcv_int[i]>=1000) ? ( (rcv_int[i]<=2000) ? (float(rcv_int[i])/1000) : value_default[i] ) : value_default[i];
	}
	attempts=0;
}

void RadioControl::print_values()
{
	for(int _idx=0;_idx<N_CHANNELS;_idx++) std::cout<< value[_idx] << '\t';
	std::cout<<'\n';
}
