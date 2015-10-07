/*
 * VexMotorController.cpp
 *
 *  Created on: Jul 17, 2015
 *      Author: Gennaro
 */

#include <stdio.h>
#include <assert.h>

#include "VexMotorController.h"

VexMotorController::VexMotorController(int pin)
{
	init(pin, FULL_REVERSE_PULSE, NEUTRAL_PULSE, FULL_FORWARD_PULSE);
	m_isInverted = false;
}

VexMotorController::~VexMotorController()
{
	haltPwm();
	mraa_pwm_close(m_pwmServoContext);
}

mraa_result_t
VexMotorController::setSpeed(double speed)
{
	if (m_pwmServoContext == NULL)
	{
		printf("PWM context is NULL.\n");
		return MRAA_ERROR_UNSPECIFIED;
	}

	if (m_isInverted) speed = -speed;

	if (speed > 1) speed = 1;
	else if (speed < -1) speed = -1;

	mraa_pwm_enable(m_pwmServoContext, 1);
	mraa_pwm_period_us(m_pwmServoContext, m_period);
	mraa_pwm_pulsewidth_us(m_pwmServoContext, calcPulse(speed));

	return MRAA_SUCCESS;
}

mraa_result_t
VexMotorController::haltPwm()
{
	if (m_pwmServoContext == NULL)
	{
		printf("PWM context is NULL.\n");
		return MRAA_ERROR_UNSPECIFIED;
	}

	return mraa_pwm_enable(m_pwmServoContext, 0);
}

int
VexMotorController::calcPulse(double speed)
{
	assert(speed >= -1 && speed <= 1);

	if (speed == 0)
	{
		return m_neutralPulseWidth;
	}
	else if (speed < 0)
	{
		return m_neutralPulseWidth - (m_neutralPulseWidth - m_fullReversePulseWidth) * speed;
	}
	else
	{
		return m_neutralPulseWidth + (m_fullForwardPulseWidth - m_neutralPulseWidth) * speed;
	}
}

void
VexMotorController::invert()
{
	m_isInverted = true;
}

void
VexMotorController::init(int pin, int fullReversePulseWidth, int neutralPulseWidth, int fullForwardPulseWidth)
{
	m_fullReversePulseWidth 	= fullReversePulseWidth;
	m_neutralPulseWidth 		= neutralPulseWidth;
	m_fullForwardPulseWidth 	= fullForwardPulseWidth;
	m_period 					= PERIOD;

	m_servoPin					= pin;

	m_pwmServoContext 			= mraa_pwm_init(m_servoPin);

	setSpeed(0);
}

