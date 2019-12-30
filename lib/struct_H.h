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

typedef struct player{
    int playerNr;
    char playerName[10];
    bool registered;
} player;

typedef struct spielFeld{
    int width;
    int height; 
    char Feld[8][9];
} spielFeld; 

typedef struct sharedMemory{
    volatile bool thinkFlag;
    int myPlayerNr;
	struct player enemy;
    char gameName[10];
    int playerCount;
    struct spielFeld feld;
    pid_t thinker;
    pid_t connector;
} sharedMemory;

