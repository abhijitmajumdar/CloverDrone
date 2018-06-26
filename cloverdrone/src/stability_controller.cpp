#include "stability_controller.h"
#include "utils.h"
#include <math.h>

Stabilize::Stabilize(int spawn_on_core, std::string imu_config_location) : is_running(true),compute_time(0),motor_time(0),arming_time(0),armed(false),
    compute_time_limit(qConstants["TIME_TO_COMPUTE"]),
    motor_time_limit(qConstants["TIME_TO_UPDATEMOTOR"]),
    arming_time_limit(qConstants["TIME_TO_ARM"])
{
	Sensor::initialize(&imu_config_location);
	Actuator::initialize();
	load_parameters();
	quadController = new qControl(&vPhi, &vTheta, &vGamma, &vMotor, &imud, &vTarget, qConstants["I_THROTTLE_TRIGGER"], qConstants["PD_THROTTLE_TRIGGER"]);
	Actuator::set_motors(vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound);
	Actuator::motors_engage(true);
  stabilizer_thread = std::thread(&Stabilize::stabilizer_routine,this,spawn_on_core);
  Utils::sleep(1000);
}

Stabilize::~Stabilize()
{
  Sensor::deinitialize();
  Actuator::deinitialize();
}

void Stabilize::stop()
{
  this->is_running = false;
  if (this->stabilizer_thread.joinable()) this->stabilizer_thread.join();
}

void Stabilize::load_parameters()
{
  vPhi.KpBuffer = qConstants["ROLL_P"];
	vPhi.KiBuffer = qConstants["ROLL_I"];
	vPhi.KdBuffer = qConstants["ROLL_D"];
	vPhi.KpAngular = qConstants["ROLL_PA"];
	vTheta.KpBuffer = qConstants["PITCH_P"];
	vTheta.KiBuffer = qConstants["PITCH_I"];
	vTheta.KdBuffer = qConstants["PITCH_D"];
	vTheta.KpAngular = qConstants["PITCH_PA"];
	vGamma.KpBuffer = qConstants["YAW_P"];
	vGamma.KiBuffer = qConstants["YAW_I"];
	vGamma.KdBuffer = qConstants["YAW_D"];
	vGamma.KpAngular = qConstants["YAW_PA"];
}

void Stabilize::arm(bool _arming)
{
  this->arming_time=this->system_time;
  this->armed=_arming;
}

// Safety: If no arming msg is received for long OR if quad if at an angle over 70deg pitch/roll then shutdown
void Stabilize::safety_check()
{
  if(check_time(arming_time,arming_time_limit) | (fabsf(imud.fusionPose.y()) > 1.2) | (fabsf(imud.fusionPose.x()) > 1.2))
  {
    Actuator::set_motors(vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound);
    this->armed=false;
    this->is_running=false;
  }
}

void Stabilize::stabilizer_routine(int run_on_core)
{
  Utils::move_to_core(run_on_core);
	while(this->is_running)
	{
    time_mutex.lock();
		this->system_time = RTMath::currentUSecsSinceEpoch();
    time_mutex.unlock();
    if (check_time(compute_time,compute_time_limit))
    {
      Sensor::IMU_update();
      this->imud = Sensor::IMU_data();
      this->quadController->compute();
      compute_time = this->system_time;
    }
    if (check_time(motor_time,motor_time_limit))
    {
      if (this->armed)
        safety_check();
      if (this->armed)
        Actuator::set_motors(vMotor.m1Value, vMotor.m2Value, vMotor.m3Value, vMotor.m4Value);
      else
        Actuator::set_motors(vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound);
      Actuator::status_led(this->armed); // Status, remove this if it takes too much time
      motor_time = this->system_time;
    }
	}
	// Make sure other threads are terminated
	this->is_running = false;
	Actuator::set_motors(vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound, vMotor.mMinBound);
  arm(false);
  Actuator::status_led(this->armed);
  Actuator::motors_engage(false);
}

void Stabilize::set_targets(float _throttle, float _theta, float _phi, float _gamma)
{
  vTarget.throttle = _throttle;
  vTarget.theta = _theta;
  vTarget.phi = _phi;
  vTarget.gamma = _gamma;
}

void Stabilize::get_accelerometer(float* x, float* y, float* z)
{
  *x = this->imud.accel.x();
  *y = this->imud.accel.y();
  *z = this->imud.accel.z();
}

void Stabilize::get_gyroscope(float* x, float* y, float* z)
{
  *x = this->imud.gyro.x();
  *y = this->imud.gyro.y();
  *z = this->imud.gyro.z();
}

void Stabilize::get_magnetometer(float* x, float* y, float* z)
{
  *x = this->imud.compass.x();
  *y = this->imud.compass.y();
  *z = this->imud.compass.z();
}

void Stabilize::get_pose(float* x, float* y, float* z)
{
  *x = this->imud.fusionPose.x();
  *y = this->imud.fusionPose.y();
  *z = this->imud.fusionPose.z();
}

void Stabilize::get_motor(float* m1, float* m2, float* m3, float* m4)
{
  *m1 = this->vMotor.m1Value;
  *m2 = this->vMotor.m2Value;
  *m3 = this->vMotor.m3Value;
  *m4 = this->vMotor.m4Value;
}

void Stabilize::print_accelerometer()
{
  std::cout << "Accelerations(x,y,z): ";
  std::cout << this->imud.accel.x() << '\t';
  std::cout << this->imud.accel.y() << '\t';
  std::cout << this->imud.accel.z() << '\n';
}

void Stabilize::print_gyroscope()
{
  std::cout << "Gyroscope(x,y,z): ";
  std::cout << this->imud.gyro.x() << '\t';
  std::cout << this->imud.gyro.y() << '\t';
  std::cout << this->imud.gyro.z() << '\n';
}

void Stabilize::print_magnetometer()
{
  std::cout << "Magnetometer(x,y,z): ";
  std::cout << this->imud.compass.x() << '\t';
  std::cout << this->imud.compass.y() << '\t';
  std::cout << this->imud.compass.z() << '\n';
}

void Stabilize::print_pose()
{
  std::cout << "Pose(x,y,z): ";
  std::cout << this->imud.fusionPose.x() << '\t';
  std::cout << this->imud.fusionPose.y() << '\t';
  std::cout << this->imud.fusionPose.z() << '\n';
}

void Stabilize::print_motor()
{
  std::cout << "Motor(M1,M2,M3,M4): ";
  std::cout << this->vMotor.m1Value << '\t';
  std::cout << this->vMotor.m2Value << '\t';
  std::cout << this->vMotor.m3Value << '\t';
  std::cout << this->vMotor.m4Value << '\n';
}
