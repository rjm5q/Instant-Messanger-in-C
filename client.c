// Ryan Morse
// CSCI 4300
// Created: 11/13/2022
// Last Edit: 11/24/2022
//
// Description: This program is a simple messenger client intended to display a basic understanding of network programming.

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

int main(int argc, char *argv[])
{
    	int sockfd, portno, n, link=1;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
	char buffer[256], *escape;

	escape = "Exit\n";
	if (argc < 3) 
	{
       		fprintf(stderr,"usage %s hostname port\n", argv[0]);
       		exit(0);
    	}
    	portno = atoi(argv[2]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	
	if (sockfd < 0)
	{      	error("ERROR opening socket");	}

    	server = gethostbyname(argv[1]);
    	if (server == NULL) 
	{
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}
    	
	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);
  	
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	{	error("ERROR connecting");	}
	printf("Connection successful! Type 'Exit' to disconnect.\n");
	
	while (link)
	{
		while (buffer[0]!='#')
		{
			bzero(buffer,256);
			fgets(buffer,255, stdin);
			n = write(sockfd,buffer,strlen(buffer));
    			if (n < 0) 
         		{	error("ERROR writing to socket");	}
			if (strcmp(buffer, escape)==0)
			{
				link = 0;
				break;
			}
		}

		bzero(buffer,256);
		while (buffer[0]!='#')
		{
			bzero(buffer,256);
    			n = read(sockfd,buffer,255);
    			if (n < 0) 
			{	error("ERROR reading from socket");	}
    			printf("[RCVD] | %s",buffer);
			if (strcmp(buffer, escape)==0)
			{
				link = 0;
				break;
			}
		}
		bzero(buffer,256);
	}
	close(sockfd);
	return 0;
}
