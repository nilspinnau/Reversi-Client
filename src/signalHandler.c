#include "../lib/signalHandler.h"
#include <stdio.h>

void handler(int sig, char field[8][8]) {
    printf("%c\n", field[0][0]);
    if(sig == SIGUSR1) {
        //think(field);
    }
}