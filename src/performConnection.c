#include "../lib/performConnection.h"
#include "../lib/handler.h"
extern sharedMemory *sm;
/*
// vielleicht noch fehlerbehandlung von send bearbeiten
bool performConnection(int socketfd, char *gameId, int playerNr) {

    setSocket(socketfd);
    if(isnext("+ MNM Gameserver v2.3 accepting connections\n")){
        toServer("VERSION 2.3\n");
        if(isnext("+ Client version accepted - please send Game-ID to join\n")){
            threeServer("ID ", gameId, "\n");
            if(isnext("+ PLAYING Reversi\n")){
                getLine();
                if(playerNr == 1) toServer("PLAYER\n");
                else toServer("PLAYER\n");
                if(isnext("- No free player\n")){
                    printf("No Playerspot availiable\n");
                    return false;
                }
                
            }
            else{
                printf("Game ID not Accepted\n");
                return false;
            }
        }
        else{
            printf("Version not Accepted\n");
            return false;
        }
    }
    else{
        printf("Server not responding\n");
        return false;
    }

    return true;
}
*/
/*
int countString(char*buffer, char*token[256]){   
    int i = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
    }
    return i-1;
}
*/
void tokenizeshit(char* buffer, char* token[256]){
    int i  = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
    }
    /*
    if(j<= i-1){
        buffer = token[j];
    }
    j++;
    return i-1;
    
    for (j=0; j<=i-1; j++) {
        printf("S:%s\n", token[j]);
    }
  */
}

bool performConnection(int socketfd, char *gameId, int playerNr, int fd[2]) {
    size_t size =sizeof(char)*256;
    char* buffer = (char*)malloc(size);
    char *token[256];
    //int numberofString;
    //int tokenIndex;
    //char* line= malloc(size);
    bool Exit= false;
    while(!Exit){
        
        read(socketfd, buffer,size);
        /*numberofString= countString(buffer,token);
        if(numberofString>0){
            ;
            numberofString--;
        }
        */
        //strtok(buffer,"\n");
        printf("S:%s", buffer);
        switch(buffer[0]){
            case '+':
                if(strcmp(buffer,"+ MNM Gameserver v2.3 accepting connections\n") == 0){
                    toServer("VERSION 2.3\n");
                    bzero(buffer, size);
                    break;
                }
                if(strcmp(buffer,"+ Client version accepted - please send Game-ID to join\n") == 0){
                    threeServer("ID ", gameId, "\n");
                    bzero(buffer, size);
                    break;
                }
                if(strcmp(buffer,"+ PLAYING Reversi\n") == 0){
                    bzero(buffer, size);
                    break;
                }
                if(strncmp(buffer,"+ Game from",11) == 0){
                    if(playerNr == 1 || playerNr ==2 ){
                         toServer("PLAYER\n");
                    }
                    bzero(buffer, size);
                    break;
                }
                if(strncmp(buffer,"+ YOU",5) == 0){

                    if(sscanf(buffer,"+ YOU %d %s player",&(sm->me.playerNr), sm->me.playerName)!= 2){
                        return false;
                    }
                    bzero(buffer, size);
                    break;
                }
                if(strncmp(buffer,"+ TOTAL",7) == 0){
                    tokenizeshit(buffer,token);
                    buffer=token[1];
                    if(sscanf(buffer,"+ %d %s player %*d",&(sm->enemy.playerNr),sm->enemy.playerName)!= 2){
                        return false;
                    }
                    
                    if(sm->me.playerNr == 0){
                    readField(sm,buffer,token,4);
                    toServer("THINKING\n");
                    }
                    
                    bzero(buffer,size);
                    //bzero(token,size);
                    break;
                }
                if(strcmp(buffer,"+ WAIT\n") == 0){
                    toServer("OKWAIT\n");
                    bzero(buffer,size);
                 
                    break;
                }
                if(strcmp(buffer,"+ MOVE 3000\n") == 0){ 
                 /*
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,1);
                    toServer("THINKING\n");
                  */ 
                    bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ MOVE 3000",11) == 0){ 
                 
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,1);
                    toServer("THINKING\n");
                    bzero(buffer,size);
                    break;
                }

                if(strncmp(buffer,"+ FIELD 8,8", 11) == 0){ 
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,0);
                    toServer("THINKING\n");
                    bzero(buffer,size);
                    break;
                }
                        
                if(strcmp(buffer,"+ OKTHINK\n")== 0){
                    sm->thinker = getppid();
                    kill(sm->thinker,SIGUSR1);
                    char themove[3];
                    read(fd[0],themove, sizeof(themove));
                    threeServer("PLAY ",themove,"\n");
                    bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ MOVEOK\n") == 0){
                    bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ GAMEOVER",10)==0){
                    Exit=true;
                    bzero(buffer,size);
                    break;
                }

            case '-':
                if(strcmp(buffer,"- TIMEOUT Be faster next time\n") == 0){
                    printf("Timeout -> Exiting Programm\n");
                    //exit(EXIT_FAILURE);
                    return Exit;
                }    
                if(strcmp(buffer,"- No free player\n") == 0){
                    printf("No Player available\n");
                    //exit(EXIT_FAILURE);
                    return Exit;
                }
                if(strcmp(buffer,"- Did not expect response from client\n") == 0){
                    printf("Wrong response\n");
                    return Exit;
                }
                if(strcmp(buffer,"- Invalid Move: Invalid position\n") == 0){
                    printf("Wrong Move\n");
                    return Exit;
                }
            default:
                return false;
            }
        }
    free(buffer);
    return Exit; 
    
}
