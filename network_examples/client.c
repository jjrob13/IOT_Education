#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

char * write_test_string = "{\"sensors\": [{\"name\":\"touch\", \"id\":0, \"value\":1}]}";
void network_write_test(int sockfd)
{
    char buffer[256];
    int n;
bzero(buffer,256);
memcpy(buffer, write_test_string, strlen(write_test_string));

while(1){
	n = write(sockfd,buffer,strlen(write_test_string));
	if (n < 0) 
		error("ERROR reading from socket");
	sleep(1);
	printf("%s\n",buffer);
}
    close(sockfd);

}

void network_read_test(int sockfd)
{
	char buffer[256];
	int n;
bzero(buffer, 256);
while(1)
{
	n = read(sockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	sleep(1);
	printf("%s\n",buffer);

}

}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

//network_write_test(sockfd);
network_read_test(sockfd);
return 0;
}
