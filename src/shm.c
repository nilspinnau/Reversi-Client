  
#include "../lib/shm.h"  
  
/*
* Datenstruktur um Spieler zu modellieren
*/
struct player {
    int playerNr;
    char *playerName;
    bool registered;
};

struct sharedMemory {
    struct player p;
    char* gameName;
    int playerNr;
    int playerCount;
    pid_t thinker;
    pid_t connector;
};

int *initialize() {
    struct sharedMemory sm;
    int shm_id = shmget(IPC_PRIVATE,sizeof(sm),0);
    int *shm_ptr = (int*) shmat(shm_id,NULL,0);
    printf("%p",shm_ptr);
    return shm_ptr;
}
