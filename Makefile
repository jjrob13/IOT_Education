CC = g++ -std=c++11
CFLAGS += -Wall -g
LDFLAGS = -lmraa -lupm-grove -lpthread -lupm-hcsr04 -lboost_system -lboost_filesystem -lboost_thread-mt -lrt -lboost_timer -lboost_chrono 

all: thread_test edison_robot

thread_test: ThreadingTest.o VexMotorController.o
	${CC} -o $@ ThreadingTest.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

ThreadingTest.o: ThreadingTest.cpp VexMotorController.h include/grove.h include/rapidjson
	${CC} -c ThreadingTest.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

edison_robot: edison_robot.o Sensor.o TouchSensor.h UltrasonicSensor.h ServoController.h
	${CC} -o $@ edison_robot.o Sensor.o ${CFLAGS} ${LDFLAGS}

Sensor.o: Sensor.h Sensor.cpp
	${CC} -c Sensor.cpp ${CFLAGS} ${LDFLAGS}

edison_robot.o: edison_robot.cpp
	${CC} -c edison_robot.cpp ${CFLAGS} ${LDFLAGS}
clean:
	rm -f example *.o *.core *.gch ./*~

