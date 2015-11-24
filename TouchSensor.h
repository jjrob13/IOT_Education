#ifndef _TOUCH_SENSOR_H
#define _TOUCH_SENSOR_H
#include "Sensor.h"
#include <iostream>
#include <mraa.hpp>
using std::cout;
using std::endl;
class TouchSensor: public Sensor{

public:

	TouchSensor(int pin, int id) : Sensor(id, "touch")
	{
		//initialize mraa object
		//TODO: potentially make the touchSensor member a reference
		this->m_touchSensor = new mraa::Gpio(pin);
		this->m_pin = pin;
		if(this->m_touchSensor->dir(mraa::DIR_IN) != mraa::SUCCESS){
			cout << "Failed to set DIR_IN" << endl;
		}
	}

	~TouchSensor()
	{
		delete this->m_touchSensor;
	}

	float
	value()
	{
		return this->m_touchSensor->read();
	}

protected:
	mraa::Gpio * m_touchSensor;
	int m_pin;	

	/*
	Routine Description:
	Read from the touch sensor and return the value
	*/
};
#endif
