CC = g++
CFLAGS += -Wall
LDFLAGS = -lmraa -lupm-grove -lpthread

all: thread_test

thread_test: ThreadingTest.o VexMotorController.o
	${CC} -o $@ ThreadingTest.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

ThreadingTest.o: ThreadingTest.cpp VexMotorController.h
	${CC} -c ThreadingTest.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core
