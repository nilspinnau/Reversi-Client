#include "../lib/performConnection.h"
#include "../lib/handler.h"
#include <sys/select.h>
#include <assert.h>
extern sharedMemory *sm;

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
int tokenizeshit(char* buffer, char* token[20]){
    int i  = 0;
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
    }
    return i;
 
}

bool performConnection(int socketfd, char *gameId, int playerNr, int fd[2]) {
    size_t size =512;
    char array[size];
    char *buffer; 
    char *token[20];
    for(int j= 0; j<=20;j++ ){
        token[j]= malloc(size);
    }
    int numberofToken;
    int tokenIndex;

    bool Exit= false;
    bool gameover=false;
    
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
            buffer = array;
            bzero(buffer, size);
            // Get data
            read(socketfd, buffer,size);
            numberofToken= tokenizeshit(buffer,token);
            for (tokenIndex=0; tokenIndex<numberofToken; tokenIndex++) {
                buffer = token[tokenIndex];
                switch(buffer[0]){
                    case '+':

                        if(strcmp(buffer,"+ MNM Gameserver v2.3 accepting connections") == 0){
                            printf("S:%s\n", buffer);
                            toServer("VERSION 2.3\n");
                            //bzero(buffer, size);
                            
                        }
                        else if(strcmp(buffer,"+ Client version accepted - please send Game-ID to join") == 0){
                            printf("S:%s\n", buffer);
                            threeServer("ID ", gameId, "\n");
                            //bzero(buffer, size);
                            
                        }
                        else if(strcmp(buffer,"+ PLAYING Reversi") == 0){
                            printf("S:%s\n", buffer);
                    
                            //bzero(buffer, size);
                            
                        }
                        else if(strncmp(buffer,"+ Game from",11) == 0){
                            printf("S:%s\n", buffer);
                            if(playerNr == 1 || playerNr ==2 ){
                                toServer("PLAYER\n");
                            }  
                            //bzero(buffer, size);
                            
                        }
                        else if(strncmp(buffer,"+ YOU",5) == 0){ 
                            if(sscanf(buffer,"+ YOU %d %s player",&(sm->me.playerNr), sm->me.playerName)!= 2){
                                return false;
                            }
                            printf("C: You are Player %d (%s)\n", (sm->me.playerNr)+1,sm->me.playerName);
                            //bzero(buffer, size);

                        }
                        else if(strncmp(buffer,"+ TOTAL",7) == 0){
                            printf("S:%s\n", buffer);
                            //bzero(buffer,size);
                            
                        }
                        else if(strncmp(buffer,"+ 1 \"W",6) == 0){
                            printf("SWHITE:%s\n", buffer);
                            if(sscanf(buffer,"+ %d %s %*s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                                return false;
                            }
                            printf("SWHITE:%s\n", buffer);
                            if(sm->me.registered == 1){
                                printf("C: Player %d (%s) is ready\n", (sm->enemy.playerNr)+1,sm->enemy.playerName);
                            }else{
                                printf("C: Player %d (%s) ist not ready\n", (sm->enemy.playerNr)+1,sm->enemy.playerName);
                            }
                            //bzero(buffer,size);
                        }
                        else if(strncmp(buffer,"+ 0 \"B",6) == 0){
                            printf("S:%s\n", buffer);
                            if(sscanf(buffer,"+ %d %s %*s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                                return false;
                            }
                            printf("SBLACK:%s\n", buffer);
                            if(sm->me.registered == 1){
                                printf("C: Player %d (%s) is ready\n", (sm->enemy.playerNr)+1,sm->enemy.playerName);
                            }else{
                                printf("C: Player %d (%s) ist not ready\n", (sm->enemy.playerNr)+1,sm->enemy.playerName);
                            }
                            //bzero(buffer,size);
                        }
                        else if(strcmp(buffer,"+ ENDPLAYERS") == 0){
                            printf("S:%s\n", buffer);
                            //bzero(buffer,size);
                            
                        }
                        else if(strcmp(buffer,"+ WAIT") == 0){
                            printf("S:%s\n", buffer);
                            toServer("OKWAIT\n");
                            //bzero(buffer,size);
                            
                        }
                        else if(strcmp(buffer,"+ MOVE 3000") == 0){ 
                            printf("S:%s\n", buffer);
                            //bzero(buffer,size);
                            
                        }
                        else if(strcmp(buffer,"+ FIELD 8,8") == 0){ 
                            printf("S:%s\n", buffer);       
                        }
                        else if(isdigit(buffer[2]) && buffer[4] != '\"') {
                            /* 
                            lese zeilennummer ein
                            schreibe feld in shm
                            */
                            int row = buffer[2] - '1';
                            printf("S:%s\n", buffer);
                            sscanf(buffer,"+ %*d %c %c %c %c %c %c %c %c",
                                &(sm->spielFeld.Feld[row][0]),&(sm->spielFeld.Feld[row][1]),
                                &(sm->spielFeld.Feld[row][2]),&(sm->spielFeld.Feld[row][3]),
                                &(sm->spielFeld.Feld[row][4]),&(sm->spielFeld.Feld[row][5]),
                                &(sm->spielFeld.Feld[row][6]),&(sm->spielFeld.Feld[row][7]));
                            
                        }
                        else if(strcmp(buffer,"+ ENDFIELD") == 0){
                            printf("S:%s\n", buffer);
                            //bzero(buffer,size);
                            if(!gameover){
                                toServer("THINKING\n");
                            }
                        }

                        else if(strcmp(buffer,"+ OKTHINK")== 0){
                            printf("S:%s\n", buffer);
                            sm->thinker = getppid();
                            kill(sm->thinker,SIGUSR1);
                        }
                        else if(strcmp(buffer,"+ MOVEOK") == 0){
                            printf("S:%s\n", buffer);
                            //bzero(buffer,size);
                        }
                        else if(strcmp(buffer,"+ GAMEOVER")==0){
                            printf("S:%s\n", buffer);
                            gameover=true;
                            //bzero(buffer,size);
                        }
                        else if(strncmp(buffer,"+ PLAYER0WON",12) == 0){
                            printf("S:%s\n", buffer);
                            
                        }
                        else if(strncmp(buffer,"+ PLAYER1WON",12) == 0){
                            printf("S:%s\n", buffer);
                        }
                        else if(strcmp(buffer,"+ QUIT")==0){
                            printf("S:%s\n", buffer);
                            exit(EXIT_SUCCESS);
                            return Exit;
                        }
                        break;
                    case '-':
                        if(strcmp(buffer,"- TIMEOUT Be faster next time") == 0){
                            printf("S:%s\n", buffer);
                            printf("Timeout -> Exiting Programm\n");
                            exit(EXIT_FAILURE);
                            return Exit;
                        }    
                        else if(strcmp(buffer,"- No free player") == 0){
                            printf("S:%s\n", buffer);
                            printf("No Player available\n");
                            exit(EXIT_FAILURE);
                            return Exit;
                        }
                        else if(strcmp(buffer,"- Did not expect response from client") == 0){
                            printf("S:%s\n", buffer);
                            printf("Wrong response\n");
                            exit(EXIT_FAILURE);
                            return Exit;
                        }
                        else if(strcmp(buffer,"- Invalid Move: Invalid position") == 0){
                            printf("S:%s\n", buffer);
                            printf("Wrong Move\n");
                            exit(EXIT_FAILURE);
                            return Exit;
                        }
                        else if (strcmp(buffer,"- Internal error. Sorry & Bye\n") == 0){
                            printf("S:%s\n", buffer);
                            printf("Server malfunction\n");
                            return Exit;
                        }    
                        break;
                    default:
                        return false;
                    }
                }
            }
        if(FD_ISSET(fd[0], &readSet)){
                char themove[3];
                read(fd[0],themove, sizeof(themove));
                threeServer("PLAY ",themove,"\n");
        }
    }    
    return Exit; 
}
