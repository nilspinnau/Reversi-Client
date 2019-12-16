#ifndef struct_H
#define struct_H
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct player{
    int playerNr;
    char playerName[10];
    bool registered;
} player;

typedef struct spielFeld{
    int width;
    int height; 
    char Feld[8][8];
} spielFeld;

typedef struct sharedMemory{
    struct player p;
    char gameName[10];
    bool awaitingMove;
    int playerNr;
    int playerCount;
    struct spielFeld feld;
    pid_t thinker;
    pid_t connector;
} sharedMemory;


#endif