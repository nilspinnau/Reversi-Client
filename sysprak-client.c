#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct sockaddr_in, serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    portno = PORTNUMBER;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    return 0;
}