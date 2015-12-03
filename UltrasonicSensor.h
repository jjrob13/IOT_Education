#ifndef _ULTRASONIC_SENSOR_H
#define _ULTRASONIC_SENSOR_H
#include "Sensor.h"
#include <iostream>
#include <mraa.hpp>
#include <upm/hcsr04.h>
#include <functional>
#include <stdexcept>
using std::cout;
using std::endl;
class UltrasonicSensor: public Sensor{

public:

/* Start static members */

	//TODO: Encapsulate this public member.  Ideally we don't need a global instance of the ultrasonic sensor, but the upm requires one for the callback,
	//currently.  There was no way to bind (I tried std::bind) an HCSR04 instance to a function call.  I already submitted a pull request to upm
	//proposing a change.
	static upm::HCSR04 * ultrasonicSensor1;	
	
	//This is a necessary interrupt callback for the ultrasonic sensor	
	static void interrupt1(void * args)
	{
		UltrasonicSensor::ultrasonicSensor1->ackEdgeDetected();
	}

	//This function is used to manage the static upm objects.
	//TODO: Eliminate this and bind upm::HCSR04 objects to a callback, eliminating static objects and callbacks
	static upm::HCSR04 * get_hcsr04(int trig_pin, int echo_pin)
	{

		//find first NULL static member, and instantiate it
		//instantiate the sensor and return the reference
		if(UltrasonicSensor::ultrasonicSensor1 == NULL)
		{
			UltrasonicSensor::ultrasonicSensor1 = new upm::HCSR04(trig_pin, echo_pin, &UltrasonicSensor::interrupt1);
			return UltrasonicSensor::ultrasonicSensor1;
		}
		else
		{
			//we were unable to find an available sensor
			throw std::runtime_error("Unable to allocate additional HCSR04 sensor.  Not enough static sensors");
		}
		return NULL;
	}

/* End static members, start public instance members */

	UltrasonicSensor(int trig_pin, int echo_pin, int id) : Sensor(id, "distance")
	{
		this->m_ultrasonicSensor = get_hcsr04(trig_pin, echo_pin);
		this->m_echo_pin = echo_pin;
		this->m_trig_pin = trig_pin;
	}

	~UltrasonicSensor()
	{
		//currently, this is a reference to a static member.
		//this works even for that case.  Again, the static member setup is certainly not ideal.
		delete this->m_ultrasonicSensor;
	}

	/*
	Routine Description:
	Read from the ultrasonic sensor and return the value
	*/
	float
	value()
	{
		return this->m_ultrasonicSensor->getDistance(CM);
	}

protected:
	int m_echo_pin, m_trig_pin;
	upm::HCSR04 * m_ultrasonicSensor;

private:

};

upm::HCSR04 * UltrasonicSensor::ultrasonicSensor1 = NULL;	

#endif

