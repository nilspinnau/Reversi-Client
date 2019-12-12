#include "../lib/handler.h"


int socketfd;
char buff[256];

void setSocket(int sockfd) {
    socketfd = sockfd;
}

bool isnext(char *s) {
    
    //bzero((char *) &buffer, sizeof(buffer));
    buff[read(socketfd, buff, 8*1024)] = '\0';
    printf("S: %s",buff);
    return (strcmp(buff, s) == 0);
}

char *getLine() {
    //bzero((char *) &buffer, sizeof(buffer));
    buff[read(socketfd, buff, 8*1024)] = '\0';
    printf("S: %s",buff);
    return buff;
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

void init(){
    //char buff[1024];
}

int cleanExit(char *s) {
    //free(buff);
    printf("%s\n", s);
    return EXIT_FAILURE;
}

void test(){
    printf("Test success");
}