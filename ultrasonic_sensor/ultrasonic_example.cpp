#include "../Sensor.h"
#include "../UltrasonicSensor.h"

#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

#define US1_TRIG_PIN 13
#define US1_ECHO_PIN 12
int main()
{
	UltrasonicSensor ultrasonic(US1_TRIG_PIN, US1_ECHO_PIN, 1);
	while(1){
		cout << ultrasonic.value() << endl;
		sleep(1);
	}

	return 0;

}
