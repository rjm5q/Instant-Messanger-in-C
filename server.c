/* Ryan Morse
 * CSCI: 4300
 * Project Part 2
 * Created: 11/24/2022
 * Last edit: 11/24/2022
 * Description: This project will demonstrate basic server capabilities in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
	perror(msg);
    	exit(1);
}

int main(int argc, char *argv[])
{
     	int sockfd, newsockfd, portno;
     	socklen_t clilen;
     	char buffer[256], *escape;

	escape = "Exit\n";
	bzero(buffer,256);
     	struct sockaddr_in serv_addr, cli_addr;
     	int n, link = 1;
     	if (argc < 2) 
	{
        	 fprintf(stderr,"ERROR, no port provided\n");
		 exit(1);
	}
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0)
	{       error("ERROR opening socket");	}
     	
	bzero((char *) &serv_addr, sizeof(serv_addr));
     	portno = atoi(argv[1]);
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
	
     	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{      	error("ERROR on binding");	}
     	listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	if (newsockfd < 0)
        {       error("ERROR on accept");       }
	
	while (link)
	{
		while (buffer[0]!='#')
		{
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
     			if (n < 0) 
			{	error("ERROR reading from socket");	}
			printf("[RCVD] | %s",buffer);
			if (strcmp(buffer, escape)==0)
			{
				link = 0;
				bzero(buffer,256);
				break;
			}
		}
		
		bzero(buffer,256);
		while (buffer[0]!='#')
		{
			bzero(buffer,256);
			fgets(buffer,255, stdin);
                        n = write(newsockfd,buffer,255);
                        if (n < 0)
                        {       error("ERROR writing to socket");     }
                        
                        if (strcmp(buffer,escape)==0)
                        {
                                link = 0;
				bzero(buffer,256);
                                break;
                        }
		}
		bzero(buffer,256);
	}
     	     	
	close(newsockfd);
     	close(sockfd);
	return 0;
}
