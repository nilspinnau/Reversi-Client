#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>


void setSocket(int socketfd);

bool isnext(char *s);

char *getLine();

void toServer(char *s);

void twoServer(char *s1, char*s2);

void threeServer(char *s1, char*s2, char *s3);

void init();

int cleanExit(char *s);

void test();