#include "SensorConfig.h"
#include "Sensor.h"
#include <vector>
#include <unistd.h>
#include <iostream>
using namespace rapidjson;
using namespace std;

int main()
{
SensorConfig c;
vector<Sensor *> sensors;

c.addUltrasonicSensor(13, 12, 1);
c.parseAndCreateSensors(sensors);

for(int i = 0; i < 10; i ++){
	cout << sensors[0]->value() << endl;
	sleep(1);
}

for(auto it = sensors.begin(); it != sensors.end(); it++){
	delete *it;
}

return 0;
}
