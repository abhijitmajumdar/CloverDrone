#include "sensors.h"
#include <thread>

static RTIMU *imu;

void Sensor::IMU_init(std::string *imu_config_location)
{
	RTIMUSettings *settings = new RTIMUSettings(imu_config_location->c_str(),"RTIMULib");
	imu = RTIMU::createIMU(settings);
	if(imu == NULL) std::cout<<"IMU not found\n";
	imu->IMUInit();
	imu->setSlerpPower(0.02);
	imu->setGyroEnable(true);
	imu->setAccelEnable(true);
	imu->setCompassEnable(true);
}

void Sensor::IMU_update(void)
{
	imu->IMURead();
}

RTIMU_DATA Sensor::IMU_data(void)
{
	return imu->getIMUData();
}

void Sensor::initialize(std::string *imu_config_location)
{
	IMU_init(imu_config_location);
}

void Sensor::deinitialize(void)
{
	delete imu;
}
