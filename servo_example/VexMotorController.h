/*
 * VexMotorController.h
 *
 *  Created on: Jul 17, 2015
 *      Author: Gennaro
 */

#ifndef VEXMOTORCONTROLLER_H_
#define VEXMOTORCONTROLLER_H_

#include <mraa.h>
#include <mraa/pwm.h>

#define FULL_REVERSE_PULSE		1000
#define NEUTRAL_PULSE			1500
#define FULL_FORWARD_PULSE		2000

#define PERIOD					20000

class VexMotorController
{
	public:
		/**
		 * Instantiates a Vex Motor Controller object.
		 *
		 * @param pin PWM pin number
		 */
		VexMotorController(int pin);

		/**
		 * Vex Motor Controller object destructor.
		 */
		~VexMotorController();

		/**
		 * Set the speed of the motor controller.
		 *
		 * @param power value in the interval [-1, 1].
		 * @return 0 on success, non-zero otherwise.
		 */
		mraa_result_t setSpeed(double speed);

		/**
		 * Halts PWM for this motor controller, allowing it to move freely.
		 */
		mraa_result_t haltPwm();

		/**
		 * Inverts the input to this motor.
		 */
		void invert();

	protected:
		int calcPulse(double speed);

		int					m_servoPin;
		mraa_pwm_context	m_pwmServoContext;

		int					m_fullReversePulseWidth;
		int					m_neutralPulseWidth;
		int					m_fullForwardPulseWidth;
		int					m_period;

		bool				m_isInverted;

	private:
			void init(int pin, int fullReversePulseWidth, int neutralPulseWidth, int fullForwardPulseWidth);
};


#endif /* VEXMOTORCONTROLLER_H_ */
