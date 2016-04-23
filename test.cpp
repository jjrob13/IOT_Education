#include "SensorConfig.h"
using namespace rapidjson;

int main()
{
SensorConfig c;

c.addUltrasonicSensor(1, 2, 3);
c.addUltrasonicSensor(4, 5, 6);
c.addLightSensor(4, 5);
c.addTouchSensor(4, 5);
c.addServo(1);
c.addServo(2);

c.saveConfigFile("ultra.json");

return 0;
}
