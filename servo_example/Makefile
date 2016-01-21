CC = g++ -std=c++11
CFLAGS += -Wall -g
LDFLAGS = -lmraa -lupm-grove -lpthread -lupm-servo

all: servo_test

calibrate_servo: calibrate_servo.o
	${CC} -o $@ calibrate_servo.o ${CFLAGS} ${LDFLAGS}
calibrate_servo.o:
	${CC} -c calibrate_servo.cpp ${CFLAGS} ${LDFLAGS}

servo_test: servo_example.o VexMotorController.o
	${CC} -o $@ servo_example.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

servo_example.o: servo_example.cpp VexMotorController.h 
	${CC} -c servo_example.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

my_servo_test: my_servo_example.o
	${CC} -o $@ my_servo_example.o ${CFLAGS} ${LDFLAGS}

my_servo_example.o: my_servo_example.cpp
	${CC} -c my_servo_example.cpp ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core ./*~

