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
int tokenizeshit(char* buffer, char* token[256]){
    int i,j  = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
    }
    if(j<= i-1){
        buffer = token[j];
    }
    j++;
    return i-1;
    /*
    for (j=0; j<=i-1; j++) {
        printf("S:%s\n", token[j]);
    }
  */
}
bool performConnection(int socketfd, char *gameId, int playerNr) {
    size_t size =sizeof(char)*256;
    char* buffer = malloc(size);
    char *token[256];
    int numberofString;
    int tokenIndex;
    //char* line= malloc(size);
    bool Exit= false;
    while(!Exit){
        
        read(socketfd, buffer,size);
        numberofString= countString(buffer,token);
        if(numberofString>0){
            ;
            numberofString--;
        }
        
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
                    buffer= NULL;
                    strtok(buffer,"\n");
                    printf("%s",buffer);
                    if(sscanf(buffer,"+ %d %s player %*d",&(sm->enemy.playerNr),sm->enemy.playerName)!= 2){
                        return false;
                    }
                    bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ WAIT\n") == 0){
                    toServer("OKWAIT\n");
                }

                break;
            case '-':
                if(strcmp(buffer,"- TIMEOUT Be faster next time") == 0){
                    printf("Timeout -> Exiting Programm\n");
                    //exit(EXIT_FAILURE);
                    return false;
                }    
                if(strcmp(buffer,"- No free player\n") == 0){
                    printf("No Player available\n");
                    //exit(EXIT_FAILURE);
                    return false;
                }
            }
        }
    free(buffer);
    return true; 
    
}
/*
     {

       printf("S: %.*s", (int) strlen(buffer), buffer);

            if(strstr(buffer, "+ MNM Gameserver")) {
                // sending client id/version
                bzero(buffer, size);
                sprintf(buffer, "VERSION %s%s", version, "\n\0");
                write(socketfd, buffer, strlen(buffer));
                printf("C: %.*s", (int) strlen(buffer), buffer);
            } else if(strstr(buffer, "+ Client version accepted - please send Game-ID to join")) {
                // sending GameId
                bzero(buffer, sizeof(buffer));
                break;       
            default:
                // disconnect, error message from server
                printf("S: %s", buffer);            
                sprintf(buffer, "ID %s%s", gameId, "\n");
                //bzero(buffer, sizeof(buffer));
                write(socketfd, buffer, strlen(buffer));
                printf("C: %.*s", (int) strlen(buffer), buffer);
            } else if(strstr(buffer, "+ PLAYING")) {
                // sending PlayerNr
                bzero(buffer, size);
                read(socketfd, buffer, size);
                printf("S: %.*s", (int) strlen(buffer), buffer);
                bzero(buffer, size);
                write(socketfd,"PLAYER\n",7*8);
                printf("C: PLAYER %d \n",playerNr);
            } else if(strstr(buffer, "+ ENDPLAYERS")) {
                // go on to readField and game in sysprak-client 
                exit(EXIT_SUCCESS);
            } else {
                // disconnect from server, error message or wrong message
                close(socketfd);
                exit(EXIT_FAILURE);
        }
            }
            bzero(buffer, size);
    }
    exit(EXIT_SUCCESS);
}

}
*/