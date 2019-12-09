#ifndef struct_H
#define struct_H

struct player;
struct sharedMemory;

struct player {
    int playerNr;
    char playerName[10];
    bool registered;
};

struct sharedMemory {
    //struct player p;
    char gameName[10];
    int playerNr;
    int playerCount;
    pid_t thinker;
    pid_t connector;
};

#endif