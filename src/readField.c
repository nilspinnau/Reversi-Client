#include "../lib/readField.h"
#include "../lib/handler.h"

bool readField(sharedMemory* sm,char *loopbuffer) {
    if(strcmp(loopbuffer,"+ FIELD 8,8")!=0)return false;
    
    for(int row = 7;row >= 0;row--){
        loopbuffer = nextbufLine();
        sscanf(loopbuffer,"+ %*c %c %c %c %c %c %c %c %c",
        &(sm->spielFeld.Feld[row][0]),&(sm->spielFeld.Feld[row][1]),
        &(sm->spielFeld.Feld[row][2]),&(sm->spielFeld.Feld[row][3]),
        &(sm->spielFeld.Feld[row][4]),&(sm->spielFeld.Feld[row][5]),
        &(sm->spielFeld.Feld[row][6]),&(sm->spielFeld.Feld[row][7]));
    }
    loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ ENDFIELD")!=0)return false;
    return true;
}