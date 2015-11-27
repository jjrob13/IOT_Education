#include "../ServoController.h"
#include <unistd.h>

int main()
{
	ServoController servo(3);
	servo.set_speed(1);
	sleep(5);
	servo.set_speed(0);
	sleep(5);
	servo.set_speed(-1);
	sleep(5);

return 0;
}
