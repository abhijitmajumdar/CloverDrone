# CloverDrone
This project contains a standalone package to provide stable flight to a quadcopter, using inputs from Radio Control(RC), tested on a Raspberry Pi 2 and 3 (RPi). It also provides a ROS workspace which contains packages to use the standalone flight stabilizer package within ROS based package, hence integrating communication over ROS, and corresponding interface. Scripts are provided to facilitate the build of the packages, configuring of ROS variables, and configuration of hardware on the Raspberry Pi.

This project is an extension of my previous work (https://github.com/abhijitmajumdar/quadController.git), and uses the same core isolation method to enable a high performance controller while still sparing enough resources to perform other tasks on a RPi.

## Requirements
- GCC
- Make
- CMake
- Git
- ROS (optional)
- RTIMULib (included as a submodule in this repository)

If using ROS, make sure it is installed on the device. You could use the following to install the other dependencies on the Raspberry Pi:
```sh
sudo apt-get install build-essential cmake git libi2c-dev
```

## Usage
#### Clone the repositories
```sh
git clone --recurse-submodules https://github.com/abhijitmajumdar/CloverDrone.git ~/CloverDrone
cd ~/CloverDrone
```
#### Configuring access to hardware peripherals
This script needs administrative privileges since it tries to configure hardware resources on the Raspberry Pi. It is meant to provide access to I2C,SPI,GPIO and USB without the need of root to run the packages. It also configures the core isolation part necessary for proper functioning of the package.
```sh
cd ~/CloverDrone
sudo ./run_to_access_resources.sh
```
You need to reboot the device after this step for changes to take effect
#### Configure ROS variables
Source the following to export ROS variables. You should edit the file according to preference, or may skip this step if you are sure of your ROS environment configurations.
```sh
cd ~/CloverDrone
source source_to_configure_ros.sh
```
#### Compiling (rtimu + cloverdrone + ros)
```sh
cd ~/CloverDrone
source source_to_build.sh
```
#### Running the package
Once all packages are compiled, the standalone cloverdrone package can be used to fly the quadcopter using the executable in the `cloverdrone/bin/` directory. It uses the `config.txt` and the `RTIMULib.ini` configuration files in the root project directory which is linked to the executable through the environment variable `CLOVERDRONEDIR`, which is automatically set when running the build script. The executable can be run as:
```sh
cd ~/CloverDrone
./cloverdrone/bin/cloverdrone
```
If the run is terminated because `CLOVERDRONEDIR` is not defined, export the environment variable to point to the path where the repository was cloned. In this case since we cloned the repository in the home directory, use the following:
```sh
cd ~/CloverDrone
export CLOVERDRONEDIR=`pwd`
```
The ROS node also uses the configuration files (`config.txt` and `RTIMULib.ini`) which are placed in the directory `cloverdrone_ros_ws/src/cloverdrone_ros/configurations`. The ROS node can be run as usual as:
```sh
rosrun cloverdrone_ros cloverdrone_ros_node
```

If ROS does not find the package, you may need to source the `setup.sh` file again:
```sh
cd ~/CloverDrone
source cloverdrone_ros_ws/devel/setup.sh
```

## Details
#### Components
- Sensing: The package uses *RTIMULib*, to interface with IMU, configured to interface over the SPI bus with the RPi. *RTIMULib* is an excellent library to interface with IMU(s) and process data with sensor fusion, with support for a broad variety of devices. This provides the flexibility to use other sensors if needed. For this project the MPU9250 was used over SPI bus 0.

- Actuation: Interface to the brushless motors was performed using a PCA9685 PWM driver, connected to the Electronic Speed Controllers (ESC) to each motor. Interface to the PCA9685 is performed over the I2C bus 1. GPIO interfaces are also used to trigger the enable on the PCA9685.

- Communication: The RC receiver signals are interpreted by an Arduino Nano and communicated over to the RPi over USB serial interface. ROS can also be configured (a script provides general configurations) to communicate over wireless network to receive commands and send status to the ROS master. We use a WiFi module for this project.

#### (Re)building
When making changes to the source files, the build script `source_to_build.sh`, provides arguments that can be passed to it to (re)build only parts of the project. In the project, *cloverdrone_ros* build packages are dependent on *cloverdrone* package build, which is in-turn dependent on *RTIMULib* build. As a result any changes made to the top of the hierarchy will only be reflected onto the lower builds when recompiled. This organization structure is pre-defined in the *CMake* files as well as the *build script*, which can be leveraged using arguments to the file. It should be noted that the ROS packages are only built if a valid installation of ROS is found in the system. Some examples are provided below:
 ```sh
 # Perform a clean build by erasing any previous build processes
 cd ~/CloverDrone
 source source_to_build.sh clean

 # Build all except ROS packages
 cd ~/CloverDrone
 source source_to_build.sh --noros

 # Re-Building to reflect changes made in RTIMULib.
 # This also removes any previous builds of cloverdrone and cloverdrone_ros
 cd ~/CloverDrone
 source source_to_build.sh imu

 # Re-Building to reflect changes made in cloverdrone.
 # This also removes any previous builds of cloverdrone_ros cloverdrone_ros
 cd ~/CloverDrone
 source source_to_build.sh cloverdrone

 # Re-Building to reflect changes made in cloverdrone_ros packages.
 # This build process uses catkin_make and hence recompiles
 cd ~/CloverDrone
 source source_to_build.sh ros

 # Re-Building to reflect changes made in RTIMULib, while removing any previous builds
 # This also removes any previous builds of RTIMULib, cloverdrone and cloverdrone_ros
 cd ~/CloverDrone
 source source_to_build.sh imu --rebuild

 # Re-Building to reflect changes made in cloverdrone, while removing any previous builds
 # This also removes any previous builds of cloverdrone and cloverdrone_ros
 cd ~/CloverDrone
 source source_to_build.sh cloverdrone --rebuild

 # Re-Building to reflect changes made in cloverdrone_ros packages, while removing any previous builds
 # This also removes any previous builds of cloverdrone_ros
 cd ~/CloverDrone
 source source_to_build.sh ros --rebuild

 # Re-Building to reflect changes made in cloverdrone, while not building ros packages
 cd ~/CloverDrone
 source source_to_build.sh cloverdrone --noros
 ```


### Changes
- added a timeout to pin setting, to ensure value is set(for example in case of arming)
- fixed pin access with large numbers(>10) and added delay in initialization
- separate configuration files for use on raspberry pi and nanopi duo
- configurable access to pins to enable motor and status led
- fixed rules file naming and sudo_user error in the script to configure non-root access
- removed the need for RTIMULib.ini file, and hard coded the desired values into the settings, declaring an overriding class. Check cloverdrone/src/sensors.cpp
- reorganized the config.ini structure completely
- the config file is now a config.ini file, which can also be parsed as an argument to the main program, if not it checks the CLOVERDRONEDIR env variable for a config.ini file
- the comm devices are now defined as a necessary parameter in the config file
- changed the configuration module to work with comments(#),spaces,tabs,empty lines etc. It now supports numeric values(int/float), flags(boolean true/false) and defines(string values) which is automatically detected and stored into corresponding maps. Access example: qConstants.flags["PARAM"]
- added parameters to class constructors to define the comm devices for peripherals (i2c,spi,serial), to be used in main
- changed to create a 'SHARED' library instead of a 'STATIC' library
- add '-j2' to catkin_make in the build script, since with more cores, npi doesnt have enough memory to compile everything simultaneously
- changes to udev rules to factor in nanopi, more generalized rules for non-sudo access, by assigning the current user group (NOTE: make sure the current user is a sudoer, check the file /etc/sudoers)
- add an option to configure the CLOVERDRONEDIR variable and ros workspace in the build script
- removed "#include <linux/i2c.h>" from "hardware_communication.cpp"
- fixed pitch inversion
- fixed default values for RC
- fixed RC interface to read data from USB properly
- fixed scaling in utils
- forgot prototype for load_defaults() in ros
- added TIME_TO_SENSE in ros config and necessary params to check
- changed Kp values in both config
- removed rosbuild type from cmake (we're using catkin_make)
- added a load_defaults() for ros_interface
- added gitignore
- Refer Changes section in readme for detailed information on each commit
