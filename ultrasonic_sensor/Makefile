CC = g++ -std=c++11
CFLAGS += -Wall -g
LDFLAGS = -lmraa -lupm-grove -lpthread -lupm-hcsr04 -lboost_system -lboost_filesystem -lboost_thread-mt -lrt -lboost_timer -lboost_chrono 

all: test_ultrasonic

test_ultrasonic: ultrasonic_example.o Sensor.o
	${CC} -o $@ ultrasonic_example.o Sensor.o ${CFLAGS} ${LDFLAGS}

ultrasonic_example.o: ultrasonic_example.cpp
	${CC} -c ultrasonic_example.cpp ${CFLAGS} ${LDFLAGS}

Sensor.o: ../Sensor.cpp 
	${CC} -c ../Sensor.cpp ${CFLAGS} ${LDFLAGS}
clean:
	rm -f example *.o *.core *.gch ./*~

