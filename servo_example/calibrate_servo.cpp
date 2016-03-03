#define SERVO_PORT 5
#include <unistd.h>
#include <mraa.hpp>
#include <upm/servo.h>

#define NEUTRAL_PULSEWIDTH_US 1500
#define PERIOD_US 20000
#include <stdio.h>

#define GPIO_PIN 13

using mraa::Pwm;
void calibrate_servo(int portno)
{
	Pwm * pwm = new Pwm(portno);
	if (pwm == NULL)
		printf("Error");

	//this is just to kill any previous servos
	pwm->enable(false);
	sleep(1);

	//this is for calibration
	pwm->enable(true);
	pwm->period_us(PERIOD_US);
	pwm->pulsewidth_us(NEUTRAL_PULSEWIDTH_US);
	
	//should be enough time
	sleep(90);
	delete pwm;


}

void sweep(int port_num)
{
	Pwm * pwm = new Pwm(port_num);
	if (pwm == NULL)
		printf("Error");
	pwm->enable(true);
	pwm->period_us(PERIOD_US);

	for(int i = 5; i <= 40; i++)
	{
	printf("i = %d\n", i);
	pwm->pulsewidth_us(i * 100);
	sleep(1);
	}

	pwm->enable(false);
	
	delete pwm;


}

void calibrate_servo_verbose()
{
	mraa::Gpio * servo_pin = new mraa::Gpio(GPIO_PIN);
	if(!servo_pin){
		printf("Failed to created servo_pin");
		delete servo_pin;
		return;
	}
	if(servo_pin->dir(mraa::DIR_OUT_HIGH) != mraa::SUCCESS){
		printf("Failed to set DIR_IN");

	}
		
	while(1)
	{
		servo_pin->write(1);
		usleep(1500);
		servo_pin->write(0);
		usleep(20000);
	}

	delete servo_pin;
}

void calibrate_servo_duty_cycle()
{
	Pwm * pwm = new Pwm(SERVO_PORT);
	float period_us = 21500, pulsewidth = 1500;
	float duty_cycle = pulsewidth / period_us;
	if (pwm == NULL)
		printf("Error");

	pwm->pulsewidth_us(period_us);
	pwm->enable(true);
	
	pwm->write(duty_cycle);
	//should be enough time
	sleep(90);
	delete pwm;
}

void test_servo(int port_number)
{
	Pwm * pwm = new Pwm(port_number);
	if (pwm == NULL)
		printf("Error");

	pwm->period_ms(20);
	pwm->enable(true);
	pwm->pulsewidth_us(2000);
	sleep(3);
	pwm->pulsewidth_us(1500);
	sleep(3);
	pwm->pulsewidth_us(1000);
	sleep(3);

	delete pwm;

}

void test_servo_verbose()
{
	Pwm * pwm = new Pwm(SERVO_PORT);
	if (pwm == NULL)
		printf("Error");

	pwm->period_ms(20);
	pwm->enable(true);
	pwm->pulsewidth_us(2000);

	delete pwm;

}

using upm::Servo;
void upm_servo_sweep()
{
	Servo * servo = new Servo(SERVO_PORT);
	if (servo == NULL)
		printf("Error");
	for(int i = 180; i >= 0; i--)
	{
		servo->setAngle(i);
		usleep(150000);
	}
	for(int i = 0; i <= 180; i++)
	{
		servo->setAngle(i);
		usleep(150000);
	}

	delete servo;
	

}

void sweep2()
{
	Pwm * pwm = new Pwm(SERVO_PORT);
	if (pwm == NULL)
		printf("Error");
	pwm->enable(true);
	for(int i = 10; i <= 30; i++)
	{
		float duty_percent = (i * 0.1)/20.0;
		printf("pulsewidth=%0.02f, duty_percent=%0.02f\n", (i * 0.1), duty_percent);
		pwm->config_percent(20, duty_percent);
		sleep(3);
	}	
	delete pwm;



}

void
spin_ccw()
{
	int pulsewidth = 1000;
	Pwm * pwm = new Pwm(SERVO_PORT);
	if (pwm == NULL)
		printf("Error");
	pwm->period_us(PERIOD_US - pulsewidth);
	pwm->pulsewidth_us(pulsewidth);
	pwm->enable(true);
	sleep(90);
	delete pwm;

}
void
spin_cw()
{
	int pulsewidth = 2000;
	Pwm * pwm = new Pwm(SERVO_PORT);
	if (pwm == NULL)
		printf("Error");
	pwm->period_us(PERIOD_US - pulsewidth);
	pwm->pulsewidth_us(pulsewidth);
	pwm->enable(true);
	sleep(90);
	delete pwm;


}

int main(int argc, char* argv[])
{
if(argc != 2)
printf("usage: %s <servo-pin>", argv[0]);
else if(argc == 2)
calibrate_servo(atoi(argv[1]));
return 0;	
}

