#include "../lib/performConnection.h"
#include "../lib/handler.h"
#include <sys/select.h>
#include <assert.h>
extern sharedMemory *sm;


int readOpponent(char *buffer) {
    sm->enemy.playerNr = *(buffer + 2) - '0';
    sm->enemy.registered = *(buffer + strlen(buffer) - 1)  - '0';
    strncat(sm->enemy.playerName, buffer + 4, strlen(buffer) - 5);

    printf("%d\n%d\n%s\n", sm->enemy.playerNr, sm->enemy.registered, sm->enemy.playerName);
    return 0;
}

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
int tokenizeshit(char* buffer, char* token[100]){
    int i  = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
        
    }
    return i;
}

int performConnection(int socketfd, char *gameId, int playerNr, int fd[2]) {
    size_t size =512;
    char array[size];
    char *buffer;
    //char *buffer = malloc(size);
    char *token[100];
    
    int numberofToken;
    int tokenIndex;
    bool exitPerf = false;
    bool gameover =false;
    while(!exitPerf){ 
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
            read(socketfd, buffer,size);

            numberofToken= tokenizeshit(buffer,token);
            for (tokenIndex=0; tokenIndex<numberofToken; tokenIndex++) {
                buffer = token[tokenIndex] ;

            switch(buffer[0]){
                case '+':
                    if(strcmp(buffer,"+ MNM Gameserver v2.3 accepting connections") == 0){
                        printf("S:%s\n", buffer);
                        toServer("VERSION 2.3\n");
                        
                    }
                    else if(strcmp(buffer,"+ Client version accepted - please send Game-ID to join") == 0){
                        printf("S:%s\n", buffer);
                        threeServer("ID ", gameId, "\n");
                        
                    }
                    else if(strcmp(buffer,"+ PLAYING Reversi") == 0){
                        printf("S:%s\n", buffer);
                        
                    }
                    else if(strncmp(buffer,"+ Game from",11) == 0){
                        printf("S:%s\n", buffer);
                        switch(playerNr) {
                            case 1:
                                toServer("PLAYER 0\n");
                                break;
                            case 2:
                                toServer("PLAYER 1\n");
                                break;
                            case 3:
                                toServer("PLAYER\n");
                                break;
                        }
                    }
                    else if(strncmp(buffer,"+ YOU",5) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ YOU %d %s",&(sm->me.playerNr), sm->me.playerName)!= 2){
                            exit(EXIT_FAILURE);
                        }

                    }
                    else if(strncmp(buffer,"+ TOTAL",7) == 0){
                        printf("S:%s\n", buffer);
                            
                    }
                    else if(strcmp(buffer,"+ ENDPLAYERS") == 0){
                        printf("S:%s\n", buffer);
                        
                    }
                    else if(strcmp(buffer,"+ WAIT") == 0){
                        printf("S:%s\n", buffer);
                        toServer("OKWAIT\n");
                        bzero(buffer,size);
                        
                    }
                    else if(strcmp(buffer,"+ MOVE 3000") == 0){ 
                        printf("S:%s\n", buffer);
                        
                    }
                    
                    else if(strcmp(buffer,"+ FIELD 8,8") == 0){ 
                        printf("S:%s\n", buffer);
                        sm->spielFeld.width = 8;
                        sm->spielFeld.height = 8;
                        
                        
                    }else if(isdigit(buffer[2]) && isdigit(buffer[strlen(buffer) - 1])) {
                        printf("S: %s\n", buffer);
                        readOpponent(buffer);
                    }
                    else if(isdigit(buffer[2]) ) {
                        // lese zeilennummer ein
                        printf("jetzt kommt zeile %i\n", buffer[2] - '0');
                        // schreibe feld in shm
                        int row = buffer[2] - '0' - 1;
                        printf("S:%s\n", buffer);
                        sscanf(buffer,"+ %*d %c %c %c %c %c %c %c %c",
                            &(sm->spielFeld.Feld[row][0]),&(sm->spielFeld.Feld[row][1]),
                            &(sm->spielFeld.Feld[row][2]),&(sm->spielFeld.Feld[row][3]),
                            &(sm->spielFeld.Feld[row][4]),&(sm->spielFeld.Feld[row][5]),
                            &(sm->spielFeld.Feld[row][6]),&(sm->spielFeld.Feld[row][7]));
                        
                    }
                    

                    else if(strcmp(buffer,"+ ENDFIELD") == 0){
                        printf("S:%s\n", buffer);
                        
                        if(!gameover){
                            toServer("THINKING\n");
                        }
                    }

                    else if(strcmp(buffer,"+ OKTHINK")== 0){
                        printf("S:%s\n", buffer);
                        sm->thinker = getppid();
                        sm->thinkFlag = true;
                        kill(sm->thinker,SIGUSR1);
                        
                    }
                    else if(strcmp(buffer,"+ MOVEOK") == 0){
                        printf("S:%s\n", buffer);
                        
                    }
                    else if(strcmp(buffer,"+ GAMEOVER")==0){
                        printf("S:%s\n", buffer);
                        gameover=true;
                        
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
                     }
                    break;
                case '-':
                    if(strcmp(buffer,"- TIMEOUT Be faster next time") == 0){
                        printf("S:%s\n", buffer);
                        printf("Timeout -> exitPerfing Programm\n");
                        exit(EXIT_FAILURE);
                    }     
                    else if(strcmp(buffer,"- No free player") == 0){
                        printf("S:%s\n", buffer);
                        printf("No Player available\n");
                        exit(EXIT_FAILURE);
                     }
                    else if(strcmp(buffer,"- Did not expect response from client") == 0){
                        printf("S:%s\n", buffer);
                        printf("Wrong response\n");
                        exit(EXIT_FAILURE);
                     }
                    else if(strcmp(buffer,"- Invalid Move: Invalid position") == 0){
                        printf("S:%s\n", buffer);
                        printf("Wrong Move\n");
                        exit(EXIT_FAILURE);
                     }
                    else if (strcmp(buffer,"- Internal error. Sorry & Bye\n") == 0){
                        printf("S:%s\n", buffer);
                        printf("Server malfunction\n");
                        exit(EXIT_FAILURE);
                    }     
                    break;
                default:
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(FD_ISSET(fd[0], &readSet)){
            char themove[3];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            }
            
    }    
    exit(EXIT_SUCCESS); 
 }
