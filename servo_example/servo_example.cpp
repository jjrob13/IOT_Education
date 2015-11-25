#include "VexMotorController.h"
#define SERVO_PORT 3
#include <unistd.h>
#include <mraa.hpp>
#include <upm/servo.h>
#include <stdio.h>
#include <stdlib.h>
using mraa::Pwm;



void pwm_test()
{
	Pwm * pwm = new Pwm(SERVO_PORT);
	if (pwm == NULL)
		printf("Error");

	pwm->enable(true);
	pwm->pulsewidth_us(2000);
	sleep(3);
	pwm->pulsewidth_us(1500);
	sleep(3);
	pwm->pulsewidth_us(1000);
	sleep(3);

	delete pwm;



}

using upm::Servo;
int main(int argc, char * argv[]){
	pwm_test();
	return 0;

}
