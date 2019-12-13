#include "../lib/signalHandler.h"

void handler(int sig, char field[8][8]) {
    if(sig == SIGUSR1) {
        *think(field);
    }
}