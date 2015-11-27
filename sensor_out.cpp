
#include <pthread.h>
#include <mraa.hpp>
#include "Sensor.h"
#include "TouchSensor.h"
#include "UltrasonicSensor.h"
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <signal.h>
#include <netdb.h>

// This is the amount of time spent waiting before sending more sensor data.
// It is in us, so a polling time of 100,000 us = 100 ms.
#define SENSOR_POLLING_TIME 100000
#define PORT_NO 8124

using std::vector;
using std::string;

int open_server_socket();
int listen_and_connect_to_client(int);
void service_client(int);
void client_disconnected(int);
void * write_sensor_data(void *);
void init_sensors_and_servos();
void cleanup_sensors_and_servos();
void add_sensor_to_stream(std::stringstream &outputJson, Sensor * sensor);
void send_string_to_client(int clientfd, const char * str_to_write);
void error(const char *msg);
/* GLOBAL VARS */
int client_connected; //indicates whether a client is currently being serviced or not
vector<Sensor*> sensors; //a vector that contains references to all sensors
/*
main:

Routine Description:

0. Initialize all sensors

1. Open server socket

2. listen for connection

3. service connection

4. close client connection when SIGPIPE signal is received

5. close serviced connection

5. Go to 2.

*/
int main()
{
	int server_sockfd, client_sockfd;
	
	init_sensors_and_servos();

	server_sockfd = open_server_socket();
	signal(SIGPIPE, client_disconnected);
	while(1)
	{
		client_sockfd = listen_and_connect_to_client(server_sockfd);
		//client_connected is a global variable used to identify when a client is currently connected and being serviced.
		client_connected = 1;
		
		//begin servicing the client on the necessary threads
		service_client(client_sockfd);
		
		//when the client is disconnected, we want to close their fd
		close(client_sockfd);

	}
	close(server_sockfd);

	cleanup_sensors_and_servos();
	return 0;
}

/*
open_server_socket():

INPUT:
None

OUTPUT:
Socket file descriptor for recently opened server socket

Routine Description:
Opens a server socket and returns the new file descriptor
*/

int
open_server_socket()
{
	struct sockaddr_in serv_addr;
	int sockfd;
	int enable = 1;
	
	//open the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	//reuse the port, even if it appears that it is taken
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NO);

	//associated this socket file descriptor with the port number specified
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	return sockfd;
}

/*
listen_and_connect_to_client():
INPUT:
int server_sockfd - the file descriptor for the server socket

OUTPUT:
int - the socket file descriptor of the client connecting to the server

Routine Description:
1. Listen for new connections

2. Connect to new connection

3. Return socket file descriptor of new client connection
*/
int 
listen_and_connect_to_client(int server_sockfd)
{
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	//listen for a single client connection
	listen(server_sockfd, 1);
	clilen = sizeof(cli_addr);

	//connect to the client and open a file descriptor to respresent it
	newsockfd = accept(server_sockfd, 
		(struct sockaddr *) &cli_addr, 
		&clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");
	
	//return the client's file descriptor
	return newsockfd;
}

/*
client_disconnected:

Routine Description:
A SIGPIPE signal indicates that the client has disconnected unexpectedly.
We want to inform all the services that this is the case, so that we can begin listening
for a new client.

*/
void
client_disconnected(int signum)
{
	if(signum == SIGPIPE)
	{
		//when the client unexpectedly disconnects (SIGPIPE), we want to stop sending data and 
		//begin listening for new clients.  This client_connected integer is used to perform this.
		client_connected = 0;
	}
}

/*
service_client:

INPUT:
int client_sockfd - The newly connected client's socket file descriptor.

OUTPUT:
None

Routine Description:

1. Start a thread for sending sensor data to the client

2. Start a thread for receiving motor commands from the client

3. Wait for threads to finish executing (the client has disconnected)

4. Return
*/
void
service_client(int client_sockfd)
{
	//this thread is intended to read data from the sensors and send it to the client
	pthread_t write_to_client;
	pthread_create(&write_to_client, NULL, write_sensor_data, (void *) &client_sockfd);
	pthread_join(write_to_client, NULL);

}

/*
write_sensor_data:
INPUT:
client_sockfd - The file descriptor of the client, which is where the data will be sent.

OUTPUT:
None

Routine Description:

0. Get the sensors for this device

1. If no client is connected, return

2. Read from each of the sensors

3. Package all of the current sensor data as JSON

4. Send the JSON to the client

5. Go to 1.
*/

void *
write_sensor_data(void * client_sockfd_)
{
	//get local integer copy of client_sockfd
	int client_sockfd = *(int *)client_sockfd_;
	std::stringstream json;
	while(client_connected)
	{
		//clear the stream
		json.str(std::string());

		//create the json string
		json << "{\"sensors\": [";
		for(int i = 0; i < sensors.size() - 1; i++)
		{
			//add sensor data to stream and put comma afterward
			add_sensor_to_stream(json, sensors[i]);
			json << ", ";
		}

		//add final sensor to json (no comma afterward)
		add_sensor_to_stream(json, sensors[sensors.size() - 1]);
		json << "]}\n";

		//send the json string to the client
		send_string_to_client(client_sockfd, json.str().c_str());
		usleep(SENSOR_POLLING_TIME);
	}
	return NULL;
}


void
add_sensor_to_stream(std::stringstream &outputJson, Sensor * sensor)
{
	outputJson << "{\"name\":\"" << sensor->name() << "\", \"id\":"
			<< sensor->id() << ", \"value\":" << sensor->value() << "}";
}

void
send_string_to_client(int clientfd, const char * str_to_write)
{
	int n = send(clientfd, str_to_write, strlen(str_to_write), 0);
	//if we fail to write, disconnect, by setting client_connected to 0
	if(n < 0)
	{
		client_connected = 0;
	}
}

/*
Routine Description:

1. Add all sensors to the global sensors vector

*/
#define TOUCH_PIN1 2
#define US1_TRIG_PIN 13
#define US1_ECHO_PIN 12
void
init_sensors_and_servos()
{
	sensors.push_back(new TouchSensor(TOUCH_PIN1, 0));
	sensors.push_back(new UltrasonicSensor(US1_TRIG_PIN, US1_ECHO_PIN, 1));
}

/*
Routine description:

Cleans up all memory associated with the Sensor objects.

*/
void
cleanup_sensors_and_servos()
{
	//Cleanup all sensor objects
	for(vector<Sensor*>::iterator it = sensors.begin(); it != sensors.end(); it++)
	{
		delete *it;
	}

	//Cleanup all ServoController objects

}

void error(const char *msg)
{
	    perror(msg);
	        exit(1);
}
