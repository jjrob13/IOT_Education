CC = g++
CFLAGS += -Wall
LDFLAGS = -lmraa -lupm-grove -lpthread -lupm-servo

all: servo_test

servo_test: servo_example.o VexMotorController.o
	${CC} -o $@ servo_example.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

servo_example.o: servo_example.cpp VexMotorController.h 
	${CC} -c servo_example.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core

