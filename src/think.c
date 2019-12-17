#include "../lib/think.h"
#include "../lib/struct_H.h"
#include <stdio.h>

char *think(sharedMemory *sm){
    struct spielFeld field = sm->feld;
    char Feld[8][8] = field->Feld;
    struct player p = sm->p;
    char playerName[10] = p->playerName;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            printf("%c",Feld[i][j]);
        }
    }

    return "E6";
}