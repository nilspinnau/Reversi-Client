#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>


void setSocket(int socketfd, char *buff);

bool isnext(char *s);

char *getLine();

void  toServer(char *s);
