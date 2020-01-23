#include "../lib/think.h"
#include "../lib/helpFunction.h"


char *think(sharedMemory *sm){ 
    int tempX=0;
    int tempY=0;
    int* x=&tempX;
    int* y=&tempY;
    if((sm->me.playerNr)==0){
        minimaxDecision(sm->spielFeld.Feld,'B',x,y);
        /*
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(validMove(sm->spielFeld.Feld, i, j, 'B')) {
                    *x = i;
                    *y = j;
                }
            }
        }
        */
    }
    if((sm->me.playerNr)==1){
        minimaxDecision(sm->spielFeld.Feld,'W',x,y);
        /*
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(validMove(sm->spielFeld.Feld, i, j, 'W')) {
                    *x = i;
                    *y = j;
                }
            }
        }
        */
    }
    switch(*x){
        case 0: 
            switch(*y){
                case 0:
                    return "A1";
                case 1:
                    return "B1";
                case 2: 
                    return "C1";
                case 3:
                    return "D1";
                case 4:
                    return "E1";
                case 5:
                    return "F1";
                case 6:
                    return "G1";
                case 7:
                    return "H1";
            }
        case 1: 
            switch(*y){
                case 0:
                    return "A2";
                case 1:
                    return "B2";
                case 2: 
                    return "C2";
                case 3:
                    return "D2";
                case 4:
                    return "E2";
                case 5:
                    return "F2";
                case 6:
                    return "G2";
                case 7:
                    return "H2";
            }
        case 2: 
            switch(*y){
                case 0:
                    return "A3";
                case 1:
                    return "B3";
                case 2: 
                    return "C3";
                case 3:
                    return "D3";
                case 4:
                    return "E3";
                case 5:
                    return "F3";
                case 6:
                    return "G3";
                case 7:
                    return "H3";
            }
        case 3: 
            switch(*y){
                case 0:
                    return "A4";
                case 1:
                    return "B4";
                case 2: 
                    return "C4";
                case 3:
                    return "D4";
                case 4:
                    return "E4";
                case 5:
                    return "F4";
                case 6:
                    return "G4";
                case 7:
                    return "H4";
            }
        case 4: 
            switch(*y){
                case 0:
                    return "A5";
                case 1:
                    return "B5";
                case 2: 
                    return "C5";
                case 3:
                    return "D5";
                case 4:
                    return "E5";
                case 5:
                    return "F5";
                case 6:
                    return "G5";
                case 7:
                    return "H5";
            }
        case 5: 
            switch(*y){
                case 0:
                    return "A6";
                case 1:
                    return "B6";
                case 2: 
                    return "C6";
                case 3:
                    return "D6";
                case 4:
                    return "E6";
                case 5:
                    return "F6";
                case 6:
                    return "G6";
                case 7:
                    return "H6";
            }
        case 6: 
            switch(*y){
                case 0:
                    return "A7";
                case 1:
                    return "B7";
                case 2: 
                    return "C7";
                case 3:
                    return "D7";
                case 4:
                    return "E7";
                case 5:
                    return "F7";
                case 6:
                    return "G7";
                case 7:
                    return "H7";
            }
        case 7: 
            switch(*y){
                case 0:
                    return "A8";
                case 1:
                    return "B8";
                case 2: 
                    return "C8";
                case 3:
                    return "D8";
                case 4:
                    return "E8";
                case 5:
                    return "F8";
                case 6:
                    return "G8";
                case 7:
                    return "H8";
            }
        default:
            exit(EXIT_FAILURE);                                     
    }
}
  