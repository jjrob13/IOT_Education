#ifndef _SERVO_CONTROLLER_H
#define _SERVO_CONTROLLER_H
#include <cmath>
#include <cassert>
#include <iostream>
#include <mraa.hpp>
#define PERIOD_US 20000
#define HIGH_PULSEWIDTH_US 1700
#define NEUTRAL_PULSEWIDTH_US 1500
#define LOW_PULSEWIDTH_US 1300
using std::cout;
using std::endl;
using std::round;
class ServoController{

public:
int m_pin;
int m_period_us;
mraa::Pwm * mraa_pwm;

ServoController(int pin)
{
	this->m_pin = pin;
	this->mraa_pwm = new mraa::Pwm(pin);
	if (this->mraa_pwm == NULL)
		printf("Error");
	this->mraa_pwm->enable(true);
	this->mraa_pwm->period_us(PERIOD_US);
	this->set_speed(0);
}

~ServoController()
{
	delete this->mraa_pwm;
}
/*
INPUT:
float speed - float in range [-1, 1], representing desired speed of servo

OUTPUT:
None

Routine Description:
This routine will set the speed of the servo to the desired speed, where 0 is no movement,
1 is max forward rotation, and -1 is max reverse rotation.
*/
void set_speed(float speed)
{
	if(speed < -1)
		speed = -1;
	if(speed > 1)
		speed = 1;

	//calculate desired pulsewidth, corresponding to float
	int pulsewidth_us;
	if(speed >= 0)
	{
		int diff = (HIGH_PULSEWIDTH_US - NEUTRAL_PULSEWIDTH_US);
		float scaled_diff = speed * diff;
		float rounded_scaled_diff = round(scaled_diff);
		pulsewidth_us = NEUTRAL_PULSEWIDTH_US + int(rounded_scaled_diff);
	}
	else
	{
		int diff = (NEUTRAL_PULSEWIDTH_US - LOW_PULSEWIDTH_US);
		float scaled_diff = speed * diff;
		float rounded_scaled_diff = round(scaled_diff);
		pulsewidth_us = NEUTRAL_PULSEWIDTH_US + int(rounded_scaled_diff);
	}

	assert(pulsewidth_us >= LOW_PULSEWIDTH_US);
	assert(pulsewidth_us <= HIGH_PULSEWIDTH_US);
	//set the correct pulsewidth to the servo
	this->mraa_pwm->pulsewidth_us(pulsewidth_us);

}
};
#endif
