#include "SensorConfig.h"
#include "Sensor.h"
#include <vector>
#include <map>
#include <unistd.h>
#include <iostream>
using namespace rapidjson;
using namespace std;

int main()
{
SensorConfig c;
vector<Sensor *> sensors;
map<int, ServoController *> servo_map;

c.addUltrasonicSensor(13, 12, 1);
c.addUltrasonicSensor(13, 12, 1);
c.addUltrasonicSensor(13, 12, 1);
c.addServo(5);
c.createSensorsAndServos(sensors, servo_map);

for(int i = 0; i < 3; i++)
{
	servo_map[5]->set_speed(1);
	sleep(3);
	servo_map[5]->set_speed(0);
	sleep(3);
	servo_map[5]->set_speed(-1);	
	sleep(3);
}

for(auto it = sensors.begin(); it != sensors.end(); it++){
	delete *it;
}

for(auto it = servo_map.begin(); it != servo_map.end(); it++){
	delete it->second;
}

return 0;
}
