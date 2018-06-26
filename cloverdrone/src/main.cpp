// System based includes
#include <iostream>
// User based includes
#include "stability_controller.h"
#include "interfaces.h"
#include "configuration.h"
#include "utils.h"

/* Global variables */
static Stabilize *base_controller;

// Function handler to execute safe exit of preogram
void prepare_exit(int s)
{
	base_controller->stop();
	std::cout<<"\nStopping...\n";
}

int main(int argc, char **argv)
{
	std::cout<<"\n\nCloverDrone\n";
	Utils::ctrl_c_exit(prepare_exit);
	std::string packagepath = Utils::get_configfile_path();
	Configuration::read_config_file(packagepath);
	base_controller = new Stabilize(3,packagepath);
	Communication *interfaces = new Communication();
	while(base_controller->running())
  {
		// Communication
		interfaces->update(base_controller->get_time());
		// Higher level control
		base_controller->set_targets(interfaces->commands.throttle,
																	interfaces->commands.pitch,
																	interfaces->commands.roll,
																	interfaces->commands.yaw);
		// Arming
    base_controller->arm(interfaces->commands.arming);
    // Debug
		if (qConstants["DEBUG"])
		{
			base_controller->print_pose();
			interfaces->print_commands();
		}
		Utils::sleep(20);
  }
	base_controller->stop();
	delete base_controller;
	delete interfaces;
	std::cout<<"\nExiting\n";
	return 0;
}
