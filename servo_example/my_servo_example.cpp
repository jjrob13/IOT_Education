#include "../ServoController.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;
int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		cout << "Usage: <prog-name> <pin-num>" << endl;
		return -1;
	}
	ServoController servo(atoi(argv[1]), true);
	for(int i = 0; i < 3; i++)
	{
		servo.set_speed(1);
		sleep(3);
		servo.set_speed(0);
		sleep(3);
		servo.set_speed(-1);	
		sleep(3);
	}
return 0;
}
