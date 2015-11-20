#ifndef _SENSOR_H
#define _SENSOR_H

#include <string>

using std::string;
class Sensor
{
public:
	Sensor(int pin, int id, string name);
	string name();
	int id();
	virtual float value() = 0;
	virtual ~Sensor() {}
protected:
	int m_id;
	string m_name;
	int m_pin;
};

#endif //_SENSOR_H
