//#include "mraa.hpp"
//#include "grove.h"

#include <mraa.hpp>
#include <upm/grove.h>

//#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <sstream>

#include <cstring> // For the memset function.
//#include <sys/socket.h> // For sockets.
#include <netdb.h>  // For sockets.

// For bluetooth.
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


// JSON Library: https://github.com/miloyip/rapidjson
#include "include/rapidjson/document.h"

#include "VexMotorController.h"

// This is the size of the buffer used to read data from ASU-VPL.
#define BUFFER_SIZE 1024
// This is the amount of time spent waiting before sending more sensor data.
// It is in us, so a polling time of 100,000 us = 100 ms.
#define SENSOR_POLLING_TIME 100000

#define FLPORT 3
#define FRPORT 5
#define RLPORT 6
#define RRPORT 9

#undef INVERT_FL
#undef INVERT_FR
#undef INVERT_RL
#undef INVERT_RR

#undef SERVO_DEBUG


int running = 1;

void sigHandler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\nClosing threads nicely\n\n");
		running = 0;
	}
}

/**
 * Writes the sensor data to the passed in string stream in the proper
 * JSON format.
 */
void addSensorToStream(std::stringstream &outputJson, const char *name, int id, double value)
{
	outputJson << "{\"name\":\"" << name << "\", \"id\":"
			<< id << ", \"value\":" << value << "}";
}

void * sensorOutputThread(void * clientfdPointer)
{
	upm::GroveButton *button = new upm::GroveButton(4);
	// This value will be set to -1 on write error.
	int bytesSent, clientfd = *(int *) clientfdPointer;

	while (running)
	{
		std::stringstream json;
		json << "{\"sensors\": [";

		int buttonValue = button->value();

		addSensorToStream(json, "touch", 0, buttonValue);

		json << "]}";
		json << "\n";

		const char *toWrite = json.str().c_str();

		//TODO: Find send declaration or implement
		bytesSent = send(clientfd, toWrite, strlen(toWrite), 0);

		if (bytesSent == -1)
			printf("Failed to send sensor data!\n");

		// 100 ms = 100,000 us.
		usleep(SENSOR_POLLING_TIME);
	}

	//returning void * type to satisfy pthread_create
	return NULL;
}

void * servoInputThread(void * clientfdPointer)
{
	int clientfd = *(int *) clientfdPointer;
	VexMotorController flMotor(FLPORT);
	VexMotorController frMotor(FRPORT);
	VexMotorController rlMotor(RLPORT);
	VexMotorController rrMotor(RRPORT);

#ifdef INVERT_FL
	flMotor.invert();
#endif

#ifdef INVERT_FR
	frMotor.invert();
#endif

#ifdef INVERT_RL
	rlMotor.invert();
#endif

#ifdef INVERT_RR
	rrMotor.invert();
#endif

	std::string *input;
	char incomingDataBuffer[BUFFER_SIZE];
	ssize_t bytesReceived;

	rapidjson::Document inputJson;

	while (running)
	{
		// We'll clear out the buffer each time to prevent data from being mixed if one message
		// is a different size (i.e. shorter) than a previous message.
		memset(&incomingDataBuffer, 0, sizeof(incomingDataBuffer));

		bytesReceived = recv(clientfd, incomingDataBuffer, sizeof(incomingDataBuffer), 0);
		//bytesReceived = read(clientfd, incomingDataBuffer, sizeof(incomingDataBuffer));

		if (bytesReceived == 0)
		{
			printf("Host shut down.\n");
			running = 0;
		}

		else if (bytesReceived == -1)
		{
			printf("Receive error.\n");
			running = 0;
		}


		else
		{
			input = new std::string(incomingDataBuffer);

			printf("Received %s\n", input->c_str());

#ifdef SERVO_DEBUG
			printf("Bytes received: %d.\n", bytesReceived);
			printf("Received string: %s\n", input->c_str());
#endif

			inputJson.Parse(input->c_str());

			rapidjson::Value &servos = inputJson["servos"];
			for (rapidjson::SizeType i = 0; i < servos.Size(); i++)
			{
				// Each value is an object.

				int servoId = servos[i]["servoId"].GetInt();


				rapidjson::Value &servoSpeedValue = servos[i]["servoSpeed"];
				//double servoSpeed = servos[i]["servoSpeed"].GetDouble();
				double servoSpeed;

				if (servoSpeedValue.IsDouble())
					servoSpeed = servoSpeedValue.GetDouble();
				else
					servoSpeed = (double)servoSpeedValue.GetInt();

#ifdef SERVO_DEBUG
				printf("Id is: %d and speed is: %f.\n", servoId, servoSpeed);
#endif

				// Execute the drive command.
				switch (servoId)
				{
					case FLPORT:
						flMotor.setSpeed(servoSpeed);
						break;
					case FRPORT:
						frMotor.setSpeed(servoSpeed);
						break;
					case RLPORT:
						rlMotor.setSpeed(servoSpeed);
						break;
					case RRPORT:
						rrMotor.setSpeed(servoSpeed);
						break;
				}
			}

			delete input;
		}
	}
	//returning void * type to satisfy pthread_create
	return NULL;
}

int getWifiClient()
{
	int status;
	struct addrinfo hostInfo;
	struct addrinfo *hostInfoList;

	// The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
	// to by hints must contain either 0 or a null pointer, as appropriate." When a struct
	// is created in C++, it will be given a block of memory. This memory is not necessary
	// empty. Therefor we use the memset function to make sure all fields are NULL.
	memset(&hostInfo, 0, sizeof(hostInfo));

	hostInfo.ai_flags = AI_PASSIVE;
	//TODO: find getaddrinfo and corresponding params
	status = getaddrinfo(NULL, "8124", &hostInfo, &hostInfoList);

	if (status != 0)
	{
		printf("Error. Failed to get address info. Error code: %d.\n", status);
		return -1;
	}

	printf("Creating a Wi-Fi socket...\n");
	int socketfd; // The socket descriptor
	socketfd = socket(hostInfoList->ai_family, hostInfoList->ai_socktype, hostInfoList->ai_protocol);
	if (socketfd == -1)
		printf("Socket error.");

	printf("Binding socket...\n");
	// Using setsockopt function to make sure the port is not in use by a
	// previous execution of our code.
	int error_code = 1;
	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &error_code, sizeof(int));

	if (status == -1)
	{
		printf("Socket is unavailable (likely already in use by a previous run of this program).\n");
		return -1;
	}

	status = bind(socketfd, hostInfoList->ai_addr, hostInfoList->ai_addrlen);

	if (status == -1)
	{
		printf("Binding error.\n");
		return -1;
	}

	printf("Listening for connections...\n");
	status = listen(socketfd, 1);

	if (status == -1)
	{
		printf("listen error.\n");
		return -1;
	}

	int clientfd;
	clientfd = accept(socketfd, NULL, NULL);

	printf("Connected to client.\n");

	return clientfd;
}

int getBTClient()
{
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buf[1024] = { 0 };
	int s, client;
	socklen_t opt = sizeof(rem_addr);

	printf("Creating a Bluetooth socket...\n");

	// allocate socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	printf("Binding socket...\n");

	// bind socket to port 1 of the first available
	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	//TODO: Resolve this issue with the address
	//loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	printf("Listening for connections...\n");

	// put socket into listening mode
	listen(s, 1);

	// accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);

	printf("Connected to client.\n");

	//ba2str( &rem_addr.rc_bdaddr, buf );
	//fprintf(stderr, "accepted connection from %s\n", buf);

	return client;
}

int main(int argc, char *argv[])
{
	pthread_t inputThread, outputThread;
	int clientfd;

	int argumentCount = argc;
	bool bluetoothEnabled = false;

	
	// Check to see if the user passed in the bluetooth flag.
	if (argumentCount > 1)
	{
		bluetoothEnabled = strcmp(argv[1], "-bt") == 0;
	}

	if (bluetoothEnabled)
	{
		clientfd = getBTClient();
	}
	else
	{
		clientfd = getWifiClient();
	}

	if (clientfd == 0 || clientfd == -1)
	{
		printf("Connection failure. Aborting.\n");
		return -1;
	}

	signal(SIGINT, sigHandler);

	pthread_create(&outputThread, NULL, sensorOutputThread, (void *) &clientfd);
	pthread_create(&inputThread, NULL, servoInputThread, (void *) &clientfd);

	pthread_join(outputThread, NULL);
	pthread_join(inputThread, NULL);

	printf("Threads have been shut down. Proceeding to close socket.\n");
	close(clientfd);

	return MRAA_SUCCESS;
}
