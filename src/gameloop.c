#include "../lib/gameloop.h"
#include "../lib/handler.h"

bool gameloop(sharedMemory* sm){

    char *loopbuffer;
    loopbuffer = getbuffer();
    if(sscanf(loopbuffer,"+ YOU %d %*s player\n",&(sm->enemy.playerNr))!= 1)return false;// not enemy + set enemy to oppposite yours
    getLine();// Line Total 2 .. Endplayer .. Field/wait
    loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ TOTAL 2")!=0)return false;
       loopbuffer = nextbufLine();
    if(sscanf(loopbuffer,"+ %*d %s player %*d",sm->enemy.playerName)!= 1)return false;// durch shared memory sm-> playerName ersetzt!
       loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ ENDPLAYERS")!=0)return false;
    if(sm->enemy.playerNr == 1){
        loopbuffer = getLine();
    }
    else{
        loopbuffer = nextbufLine();
    }

    bool exit = false;
    //printf("%s", loopbuffer);

    while(!exit){
        if(strcmp(loopbuffer,"+ WAIT\n") == 0){
            toServer("OKWAIT\n");
        }
        if(strcmp(loopbuffer,"GAMEOVER\n") == 0){
            readField(sm,loopbuffer);
            //handle who is winner
            break;
        }
        if(strcmp(loopbuffer,"- TIMEOUT Be faster next time\n") == 0){
            printf("Timeout -> Exiting Programm");
            break;
        }
        if(strcmp(loopbuffer,"+ MOVE 3000") == 0){ //nur beim 1. mal -Teil von größerem String
            loopbuffer = nextbufLine();
            if(!readField(sm,loopbuffer)){
                printf("Field could not be read");
                break;
            }
            toServer("THINKING\n");
            //to test (working)
            for(int i = 0;i < 8;i++){
                for(int j= 0;j < 8;j++){
                    printf("%c",sm->feld.Feld[i][j]);
                }
                printf("\n");
            }
            getLine();
            //break;//entfernen wenn thinker funtioniert
            //thinker anstoßen
            toServer("PLAY F4\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move");
                break;
            }
        

        }

        if(strcmp(loopbuffer,"+ MOVE 3000\n") == 0){
            getLine();
            resetLinebuf();
            loopbuffer = nextbufLine();
            printf("%s",loopbuffer);
            if(!readField(sm,loopbuffer)){
                printf("Field could not be read");
                return false;
            }
            toServer("THINKING\n");
            for(int i = 0;i < 8;i++){
                for(int j= 0;j < 8;j++){
                    printf("%c",sm->feld.Feld[i][j]);
                }
                printf("\n");
            }
            getLine();
            //break;//entfernen wenn thinker funtioniert
            //thinker anstoßen
            toServer("PLAY B6\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move");
                return false;
            }
        

        }    
        loopbuffer = getLine();        
    }
    return false;

}
