#ifndef _SERVO_CONTROLLER_H
#define _SERVO_CONTROLLER_H
#include <cmath>
#include <cassert>
#include <iostream>
#include <mraa.hpp>
#include <math.h>
#define PERIOD_US 20000
#define HIGH_PULSEWIDTH_US 1850
#define NEUTRAL_PULSEWIDTH_US 1500
#define LOW_PULSEWIDTH_US 1150
using std::cout;
using std::endl;
using std::round;
class ServoController{

public:
int m_pin;
int m_period_us;
bool m_invert;
mraa::Pwm * mraa_pwm;

ServoController(int pin, bool invert = false)
{
	this->m_pin = pin;
	this->m_invert = invert;
	this->mraa_pwm = new mraa::Pwm(pin);
	if (this->mraa_pwm == NULL)
		printf("Error");
	//init pwm communication
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
	if(this->m_invert)
		speed = -speed;

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
	//set the correct pulsewidth and period to the servo
	this->mraa_pwm->pulsewidth_us(pulsewidth_us);

	cout << "Updating speed for servo on pin " << this->m_pin << " to " << speed << endl;

}
};
#endif
