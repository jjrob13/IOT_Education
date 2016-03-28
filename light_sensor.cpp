#include <mraa.hpp>
#include <iostream>
#include <ios>
#include <stdio.h>
#include "LightSensor.h"

using namespace std;

#define NUM_SECONDS 90
int main(int argc, char ** argv)
{
	if(argc != 2){
		cout << "Usage: " << argv[0] << " <pin-num>" << endl;
		return -1;
	}

	LightSensor sensor(atoi(argv[1]), 3);

	for(int i = 0; i < NUM_SECONDS; i++){
		cout << sensor.value() << endl;
		sleep(1);
	}

	return 0;
}
