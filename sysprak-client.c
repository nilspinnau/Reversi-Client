#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "performConnection.h"

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char **argv) {

    int opt;
    int gameId;
    int playerNr;

     while ((opt = getopt (argc, argv, "g:p:")) != -1) {
        switch (opt) {
            case 'g':
 	            gameId = atoi(optarg);
	            break;
            case 'p':
                    playerNr = atoi(optarg);
	            break;
        }
    }
<<<<<<< HEAD
    int sockfd, portno;
    struct sockaddr_in serv_addr;

=======
    int sockfd, portno; //,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //char buffer[256];
    portno = PORTNUMBER;
>>>>>>> d55f394c5bc138b635158c6d2d88e057545a4b53
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");
    bzero((char* ) &serv_addr, sizeof(serv_addr));
    

    
    server = gethostbyname(HOSTNAME);
    if (server == NULL) {
        perror("ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server-> h_length);
    serv_addr.sin_port = htons(portno);
<<<<<<< HEAD
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {
	perror("ERROR connecting");
	}
    performConnection(sockfd);
    printf("%i",gameId);
=======
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
    }
    /*bzero(buffer,256);
    fgets(buffer,255,stdin); 
    
    n = read(sockfd,buffer,255);
    if (n < 0)
        perror("ERROR reading from socket");
    printf("%s\n",buffer);
    */
    performConnection(sockfd);
    
    /*n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        perror("ERROR writing to socket");
    bzero(buffer,256);*/
    
>>>>>>> d55f394c5bc138b635158c6d2d88e057545a4b53
    return 0;
}
