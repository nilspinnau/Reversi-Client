#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char **argv) {
    long i = atoi(argv[1]);
    if (argc != 2) {
        printf("Bitte geben Sie eine 13-stellige Game-ID und die gewünschte Spielernummer an");
        return -1;
    }
    if (i < 1 || i > 2) {
        printf("Spielerzahl muss entweder 1 oder 2 sein");
        return -1;
    }
    if (strlen(argv[0]) != 13) {
        printf("Game-ID muss 13-stellig sein");
        return -1;
    }
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char* ) &serv_addr, sizeof(serv_addr));
    portno = PORTNUMBER;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0)
            printf("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        printf("ERROR on accept");

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) printf ("ERROR reading from socket");
    printf("Here is the messsage: %s\n",buffer);
    n = write(newsockfd,"I got your message", 18);
    if (n < 0) printf ("ERROR writing to socket");
    return 0;
}