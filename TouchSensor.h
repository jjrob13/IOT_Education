#ifndef _TOUCH_SENSOR_H
#define _TOUCH_SENSOR_H
#include "Sensor.h"
#include <iostream>
#include <mraa.hpp>
using std::cout;
using std::endl;
class TouchSensor: public Sensor{

public:

	TouchSensor(int pin, int id, string name) : Sensor(pin, id, name)
	{
		//initialize mraa object
		//TODO: potentially make the touchSensor member a reference
		this->m_touchSensor = new mraa::Gpio(pin);
		if(this->m_touchSensor->dir(mraa::DIR_IN) != mraa::SUCCESS){
			cout << "Failed to set DIR_IN" << endl;
		}
	}

	~TouchSensor()
	{
		delete this->m_touchSensor;
	}


protected:
	mraa::Gpio * m_touchSensor;
	

	/*
	Routine Description:
	Read from the touch sensor and return the value


	*/
	float
	value()
	{
		return this->m_touchSensor->read();
	}
};
#endif
