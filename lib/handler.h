#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#endif

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