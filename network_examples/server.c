/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void read_from_file(char*, char*);
void error(const char *msg)
{
	    perror(msg);
	        exit(1);
}


int
my_write(int socket, char * string, int len)
{
	printf("%s", string);
	return write(socket, string, len);

}
void
write_test(int socketfd)
{
	char buffer[256];
	int n, i;
	char *pos_filename = "pos_json.txt";
	char pos_json_string[255];
	char *neg_filename = "neg_json.txt";
	char neg_json_string[255];
	read_from_file(pos_filename, pos_json_string);
	read_from_file(neg_filename, neg_json_string);
	for(i = 0;1; i++)
	{

		if(i % 10 == 0)
		{
			n = my_write(socketfd, pos_json_string, strlen(pos_json_string));
			if (n < 0) error("ERROR writing to socket");
		}else
		{
			n = my_write(socketfd, neg_json_string, strlen(neg_json_string));
		}
		sleep(1);

	}

}

void
write_touch_test(int socketfd)
{
	char buffer[256];
	int n, i;
	char *pos_filename = "pos_touch.txt";
	char pos_json_string[255];
	char *neg_filename = "neg_touch.txt";
	char neg_json_string[255];
	read_from_file(pos_filename, pos_json_string);
	read_from_file(neg_filename, neg_json_string);
	for(i = 0;1; i++)
	{

		if(i % 10 == 0)
		{
			n = my_write(socketfd, pos_json_string, strlen(pos_json_string));
			if (n < 0) error("ERROR writing to socket");
		}else
		{
			n = my_write(socketfd, neg_json_string, strlen(neg_json_string));
		}
		sleep(1);

	}

}

void
read_test(int newsockfd)
{
	char buffer[256];
	int n;
	while(1)
	{
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		printf("Here is the message: %s\n",buffer);
		sleep(1);
	}

}

void
connect_to_client(int * sockfd, int * newsockfd, int portno)
{
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(*sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(*sockfd,5);
	clilen = sizeof(cli_addr);
	*newsockfd = accept(*sockfd, 
		(struct sockaddr *) &cli_addr, 
		&clilen);
	if (*newsockfd < 0) 
		error("ERROR on accept");


}

void
read_from_file(char * filename, char * file_contents)
{
	FILE * f = fopen(filename, "r");
	fgets(file_contents, 255, f);
	fclose(f);

}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;

	if (argc < 3) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	portno = atoi(argv[1]);
	connect_to_client(&sockfd, &newsockfd, portno);
	if(atoi(argv[2]) == 1)
		write_test(newsockfd);
	else if(atoi(argv[2]) == 2)
		write_touch_test(newsockfd);
	else if(atoi(argv[2]) == 3)
		read_test(newsockfd);
	close(sockfd);
	close(newsockfd);
	return 0; 
}
