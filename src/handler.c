#include "../lib/handler.h"


int socketfd;
char *buffer;

void setSocket(int sockfd, char *buff) {
    buffer = buff;
    socketfd = sockfd;
}

bool isnext(char *s) {
    bzero((char *) &buffer, sizeof(buffer));
    read(socketfd, buffer, sizeof(buffer));
    return (strstr(buffer, s) != NULL);
}

char *getLine() {
    bzero((char *) &buffer, sizeof(buffer));
    read(socketfd, buffer, sizeof(buffer));
    return buffer;
}

void toServer(char *s) {
    write(socketfd, s, sizeof(s));
}


