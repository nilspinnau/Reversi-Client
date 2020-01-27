#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

#define MAX (10)

typedef struct player{
    int playerNr;
    char playerName[256];
    int registered;
} player;

typedef struct move {
    int x;
    int y;
    int val;
} move;

typedef struct spielFeld{
    int width;
    int height; 
    char Feld[8][8];
} spielFeld; 

typedef struct alarmStruct {
    bool alarm;
    int bestX;
    int bestY;
    int numberOfMoves;
} alarmStruct;

typedef struct threadArguments {
    char whoseTurn;
    int i;
    move pos[60];
    char feld[8][8];
} threadArguments;

typedef struct sharedMemory{
    volatile bool thinkFlag;
    struct spielFeld spielFeld;
    struct player enemy;
    struct player me;
    struct alarmStruct alarm;
    int playerCount;
	char gameName[10];
    pid_t thinker;
    pid_t connector;
} sharedMemory;

