#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>


void setSocket(int socketfd);

/* char *getLine();
 */
void  toServer(char *s);

void threeServer(char *s1, char*s2, char *s3);

void init();

int cleanExit(char *s);

bool isnext(char *s);

char *getLine();

char *getbuffer();

char *nextbufLine();

void resetLinebuf();