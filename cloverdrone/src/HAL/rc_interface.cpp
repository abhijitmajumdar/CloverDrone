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
		std::string rcv_str = std::string((char*)rcv_bytes);
		for(int i=0;i<N_CHANNELS;i++)
		{
			if(rcv_str.length()>0)
			{
				rcv_int[i] = std::stoi(rcv_str);
				rcv_str.erase(0,5);
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
