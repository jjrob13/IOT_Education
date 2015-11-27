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
int m_high_pulsewidth_us;
int m_neutral_pulsewidth_us;
int m_low_pulsewidth_us;
mraa::Pwm * m_pwm;

ServoController(int pin)
{
	/*
	this->m_pin = pin;
	this->m_period_us = PERIOD_US;
	this->m_high_pulsewidth_us = HIGH_PULSEWIDTH_US;
	this->m_neutral_pulsewidth_us = NEUTRAL_PULSEWIDTH_US;
	this->m_low_pulsewidth_us = LOW_PULSEWIDTH_US;
	*/
	ServoController(pin, PERIOD_US, HIGH_PULSEWIDTH_US, NEUTRAL_PULSEWIDTH_US, LOW_PULSEWIDTH_US);
}

ServoController(int pin, int period_us, int high_pulsewidth_us, int neutral_pulsewidth_us, int low_pulsewidth_us)
{
	//input checking
	assert(high_pulsewidth_us >= neutral_pulsewidth_us && neutral_pulsewidth_us >=  low_pulsewidth_us);
		
	this->m_pin = pin;
	this->m_period_us = period_us;
	this->m_high_pulsewidth_us = high_pulsewidth_us;
	this->m_neutral_pulsewidth_us = neutral_pulsewidth_us;
	this->m_low_pulsewidth_us = low_pulsewidth_us;
	this->m_pwm = new mraa::Pwm(pin);
	if (this->m_pwm == NULL)
		printf("Error");
	this->m_pwm->enable(true);
	this->m_pwm->period_us(period_us);
	this->set_speed(0);

}

~ServoController()
{
	delete this->m_pwm;
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
		int diff = (this->m_high_pulsewidth_us - this->m_neutral_pulsewidth_us);
		cout << "diff = " << diff << endl;
		float scaled_diff = speed * (this->m_high_pulsewidth_us - this->m_neutral_pulsewidth_us);
		cout << "scaled_diff = " << scaled_diff << endl;
		float rounded_scaled_diff = round(scaled_diff);
		cout << "rounded_scaled_diff = " << rounded_scaled_diff << endl;
		pulsewidth_us = this->m_neutral_pulsewidth_us + int(round(speed * (this->m_high_pulsewidth_us - this->m_neutral_pulsewidth_us)));
		
		cout << "pulsewidth_us = " << pulsewidth_us << endl;
	}
	else
	{
		pulsewidth_us = this->m_neutral_pulsewidth_us - int(round(speed * (this->m_neutral_pulsewidth_us - this->m_low_pulsewidth_us)));
	}

	//assert(pulsewidth_us >= this->m_low_pulsewidth_us);
	//assert(pulsewidth_us <= this->m_high_pulsewidth_us);
	//set the correct pulsewidth to the servo
	//this->m_pwm->pulsewidth_us(pulsewidth_us);

}
};
#endif
