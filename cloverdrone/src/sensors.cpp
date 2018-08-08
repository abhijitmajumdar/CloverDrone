#include "sensors.h"
#include "utils.h"

static RTIMU *imu;

class RTIMUSettings_Raw : public RTIMUSettings
{
	public:
		RTIMUSettings_Raw() : RTIMUSettings("/tmp/","RTIMULib")
		{
			// Dont want to save or load settings from a setting file
			// dump it in /tmp to be removed automatically and load
			// default values to be modified as per requirements
			setDefaults();
		}
};

void Sensor::IMU_init(int spi_device, int spi_select)
{
	RTIMUSettings_Raw *settings = new RTIMUSettings_Raw();
	settings->m_imuType = 7;
	settings->m_fusionType = 2;
	settings->m_busIsI2C = false;
	settings->m_SPIBus = spi_device;
	settings->m_SPISelect= spi_select;
	settings->m_SPISpeed = 8000000;
	settings->m_axisRotation = 0;
	settings->m_pressureType = 1;
	settings->m_humidityType = 1;
	settings->m_MPU9250GyroAccelSampleRate = 400;
  settings->m_MPU9250CompassSampleRate = 40;
  settings->m_MPU9250GyroLpf = 1;
  settings->m_MPU9250AccelLpf = 3;
  settings->m_MPU9250GyroFsr = 16;
  settings->m_MPU9250AccelFsr = 16;
	imu = RTIMU::createIMU(settings);
	if(imu == NULL) Utils::throw_error("IMU not found\n");
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

void Sensor::initialize(std::string device)
{
	int dev=0,sel=0;
	std::vector<std::string> s_list = Utils::split_str(Utils::last_in_str(device,"/dev/spidev"),".");
	if(s_list.size()==2){
    if(isdigit(s_list[0][0]) & isdigit(s_list[1][0])){
      dev = std::atoi(s_list[0].c_str());
			sel = std::atoi(s_list[1].c_str());
    }
  }
	IMU_init(dev,sel);
}

void Sensor::deinitialize(void)
{
	delete imu;
}
