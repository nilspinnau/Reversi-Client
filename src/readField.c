#include "../lib/readField.h"
#include "../lib/handler.h"

bool readField(sharedMemory* sm,char *loopbuffer) {
    if(strcmp(loopbuffer,"+ FIELD 8,8")!=0)return false;
    
    for(int row = 0;row < 8;row++){
        loopbuffer = nextbufLine();
        sscanf(loopbuffer,"+ %*c %c %c %c %c %c %c %c %c",
        &(sm->feld.Feld[row][0]),&(sm->feld.Feld[row][1]),&(sm->feld.Feld[row][2]),
        &(sm->feld.Feld[row][3]),&(sm->feld.Feld[row][4]),&(sm->feld.Feld[row][5]),
        &(sm->feld.Feld[row][6]),&(sm->feld.Feld[row][7]) );
    }
    loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ ENDFIELD")!=0)return false;
    return true;
}