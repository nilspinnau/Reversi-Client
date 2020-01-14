#include "../lib/performConnection.h"
#include "../lib/handler.h"
#include <sys/select.h>
#include <assert.h>
extern sharedMemory *sm;

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
/* add a fd to fd_set, and update max_fd */
int safe_fdSet(int fd, fd_set* fdSet, int* max_fd){
    assert(max_fd != NULL);
    FD_SET(fd,fdSet);
    if(fd > *max_fd){
        *max_fd= fd;
    }
    return 0;
}
int safe_fdClear(int fd, fd_set* fdSet, int* max_fd){
    assert(max_fd != NULL);
    FD_CLR(fd,fdSet);
    if(fd == *max_fd){
        (*max_fd)--;
    }
    return 0;
}
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
        
    fd_set readSet;
        int max_fd= -1;
        FD_ZERO(&readSet);
        safe_fdSet(fd[0],&readSet,&max_fd);
        safe_fdSet(socketfd,&readSet,&max_fd);
    // note the max_fd+1
    if(select(max_fd+1,&readSet,NULL,NULL,NULL)<=0){
            perror("Select\n");
    }
    if (FD_ISSET(socketfd, &readSet)){ 
      
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
                    /*
                    char themove[3];
                    read(fd[0],themove, sizeof(themove));
                    threeServer("PLAY ",themove,"\n");
                    */
                    bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ MOVEOK\n") == 0){
                    bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ GAMEOVER",10)==0){
                    printf("Hi\n");
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
                if(strcmp(buffer,"- Internal error. Sorry & Bye\n") == 0){
                    printf("Server malfunction\n");
                    return Exit;
                }    
            default:
                return false;
            }
            
        }
        if(FD_ISSET(fd[0], &readSet)){
            char themove[3];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            }
            
    }    
    free(buffer);
    return Exit; 
}
