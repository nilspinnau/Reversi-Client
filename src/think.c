#include "../lib/think.h"


char *think(sharedMemory *sm){
    spielFeld feld = sm->feld;
    char Feld[8][8];
    memcpy(Feld,feld.Feld,64*sizeof(char));
    char answer[2] = "";
    player p = sm->p;
    char playerName[10];
    memcpy(playerName,p.playerName,10*sizeof(char));
    char playerNr = (char) p.playerNr;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (Feld[i][j] == 'B') {
                Feld[i][j] = '0';
            }
            if (Feld[i][j] == 'W') {
                Feld[i][j] = '1';
            }
            if (p.playerNr == '0') {
                if (Feld[i][j] == '*') {
                    if ((Feld[i][j-2] == '0' && Feld[i][j-1] == '1') ||
                        (Feld[i][j+2] == '0' && Feld[i][j+1] == '1') ||
                        (Feld[i-2][j] == '0' && Feld[i-1][j] == '1') ||
                        (Feld[i+2][j] == '0' && Feld[i+1][j] == '1') ||
                        (Feld[i-2][j-2] == '0' && Feld[i-1][j-1] == '1') ||
                        (Feld[i+2][j+2] == '0' && Feld[i+1][j+1] == '1')) {
                            char line = i + '0';
                            char column = j + '0';
                            strncat(answer,&line,1);
                            strncat(answer,&column,1);
                    }
                }
            }
            else {
                if (Feld[i][j] == '*') {
                    if ((Feld[i][j-2] == '1' && Feld[i][j-1] == '0') ||
                        (Feld[i][j+2] == '1' && Feld[i][j+1] == '0') ||
                        (Feld[i-2][j] == '1' && Feld[i-1][j] == '0') ||
                        (Feld[i+2][j] == '1' && Feld[i+1][j] == '0') ||
                        (Feld[i-2][j-2] == '1' && Feld[i-1][j-1] == '0') ||
                        (Feld[i+2][j+2] == '1' && Feld[i+1][j+1] == '0')) {
                            char line = i + '0';
                            char column = j + '0';
                            strncat(answer,&line,1);
                            strncat(answer,&column,1);
                    }
                }
            }
        }
    }
    return *answer;
}