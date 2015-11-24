CC = g++ -std=c++11
CFLAGS += -Wall
LDFLAGS = -lmraa -lupm-grove -lpthread -lupm-hcsr04

all: thread_test sensor_out

thread_test: ThreadingTest.o VexMotorController.o
	${CC} -o $@ ThreadingTest.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

ThreadingTest.o: ThreadingTest.cpp VexMotorController.h include/grove.h include/rapidjson
	${CC} -c ThreadingTest.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

sensor_out: sensor_out.o Sensor.o TouchSensor.h UltrasonicSensor.h
	${CC} -o $@ sensor_out.o Sensor.o ${CFLAGS} ${LDFLAGS}

Sensor.o: Sensor.h Sensor.cpp
	${CC} -c Sensor.cpp ${CFLAGS} ${LDFLAGS}

sensor_out.o: sensor_out.cpp
	${CC} -c sensor_out.cpp ${CFLAGS} ${LDFLAGS}
clean:
	rm -f example *.o *.core *.gch

