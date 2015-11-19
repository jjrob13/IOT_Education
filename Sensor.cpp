#include "Sensor.h"

Sensor::Sensor(int pin, int id, string name)
{
	this->m_pin = pin;
	this->m_id = id;
	this->m_name = name;

}


int
Sensor::id()
{
	return this->m_id;
}

string
Sensor::name()
{
	return this->m_name;
}
