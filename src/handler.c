#include "../lib/handler.h"


int socketfd;
char buff[256];

void setSocket(int sockfd) {
    socketfd = sockfd;
}

void toServer(char *s) {
    write(socketfd, s, strlen(s));
    printf("C: %s",s);
}

void twoServer(char *s1, char*s2) {
    strcpy(buff,s1);
    strcat(buff,s2);
    write(socketfd, buff, strlen(buff));
    printf("C: %s",buff);
}


void threeServer(char *s1, char *s2, char *s3) {
    strcpy(buff,s1);
    strcat(buff,s2);
    strcat(buff,s3);
    write(socketfd, buff, strlen(buff));
    printf("C: %s",buff);
}
