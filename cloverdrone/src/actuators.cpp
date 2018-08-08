#include "actuators.h"
#include "pca9685.h"
#include "gpio_access.h"

static PCA9685* motors;
static GPIO* motors_en;
static GPIO* led;

void Actuator::initialize(std::string device, int motors_enable_pin, int status_led_pin)
{
	motors = new PCA9685(device,0x40,50);
	motors_en = new GPIO(motors_enable_pin, OUT);
	led = new GPIO(status_led_pin, OUT);
}

void Actuator::motors_engage(bool enable)
{
	motors_en->set(!enable);
}

void Actuator::status_led(bool enable)
{
	led->set(!enable);
}

void Actuator::set_motors(float m1, float m2, float m3, float m4)
{
	motors->set_servo_signal(3,&m1);
	motors->set_servo_signal(4,&m2);
	motors->set_servo_signal(5,&m3);
	motors->set_servo_signal(6,&m4);
}

void Actuator::deinitialize(void)
{
	delete motors;
	delete motors_en;
	delete led;
}
