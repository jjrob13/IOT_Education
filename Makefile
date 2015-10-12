CC = g++
CFLAGS += -Wall
LDFLAGS = -Llib/upm/build/src/grove -lmraa -lupm-grove -lpthread

all: thread_test

thread_test: ThreadingTest.o VexMotorController.o lib/mraa/build/src/libmraa.so lib/upm/build/src
	${CC} -o $@ ThreadingTest.o VexMotorController.o ${CFLAGS} ${LDFLAGS}

ThreadingTest.o: ThreadingTest.cpp VexMotorController.h
	${CC} -c ThreadingTest.cpp ${CFLAGS} ${LDFLAGS}

VexMotorController.o: VexMotorController.cpp
	${CC} -c VexMotorController.cpp ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core

lib/upm/build/src: lib/upm/build
	cd lib/upm/build; \
	cmake .. -DBUILDSWIGNODE=OFF;\
	make;

lib/upm/build: lib/mraa/build/src/libmraa.so lib/upm
	mkdir lib/upm/build

lib/mraa/build/src/libmraa.so: lib/mraa/build
	cd lib/mraa/build; \
	cmake ..;\
	make mraa;\
	make install;

lib/mraa/build: lib/mraa
	mkdir lib/mraa/build
