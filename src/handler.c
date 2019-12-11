#include "../lib/handler.h"


int socketfd;
char *buffer;

void setSocket(int sockfd, char *buff) {
    buffer = buff;
    socketfd = sockfd;
}

/* bool isnext(char *s) {
    bzero((char *) &buffer, sizeof(buffer));
    read(socketfd, buffer, sizeof(buffer));
    bool ret = strcmp(buffer, s) == 0;
    return ret;
} */

/* char *getLine() {
    bzero((char *) &buffer, sizeof(buffer));
    read(socketfd, buffer, sizeof(buffer));
    return buffer;
} */

void toServer(char *s) {
    write(socketfd, s, sizeof(s));
}

void twoServer(int sf, char *s1, char *s2) {
    char buffer[256] = {0};
    sprintf(buffer, s1, s2, "%s", "\n\0");
    write(sf, buffer, strlen(buffer));
}
void getLine(int sf, char *buffer) {
    bzero((char *) &buffer, sizeof(buffer));
    read(sf, buffer, sizeof(buffer));
    printf("%.*s", (int) strlen(buffer), buffer);
} 

void line(int sf) {
    char buffer[256] = {0};
    read(sf, buffer, sizeof(buffer));
    printf("%.*s", (int) strlen(buffer), buffer);
}

void isnext(int sf, bool *rt, char *s) {
    char buffer[256] = {0};
    read(sf, buffer, sizeof(buffer));
    printf("%s", buffer);
    *rt = (strstr(buffer, s) != NULL);
}
 

bool isNext(int sf, char *s) {
    read(sf, buffer, sizeof(buffer));
    bool *ret = malloc(8);
    *ret = strstr(buffer, s) != 0;
    return ret; 
    free(ret);
}



