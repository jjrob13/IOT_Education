#ifndef _LIGHT_SENSOR_H
#define _LIGHT_SENSOR_H
#include "Sensor.h"
#include <iostream>
#include <mraa.hpp>
using std::cout;
using std::endl;
class LightSensor: public Sensor{

public:

	LightSensor(int pin, int id) : Sensor(id, "light")
	{
		//initialize mraa object
		this->m_lightSensor = new mraa::Aio(pin);
		this->m_pin = pin;
	}

	~LightSensor()
	{
		delete this->m_lightSensor;
	}

	float
	value()
	{
		return this->m_lightSensor->read();
	}

protected:
	mraa::Aio * m_lightSensor;
	int m_pin;	

	/*
	Routine Description:
	Read from the touch sensor and return the value
	*/
};
#endif

