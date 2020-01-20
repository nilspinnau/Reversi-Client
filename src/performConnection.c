#include "../lib/performConnection.h"
#include "../lib/handler.h"
#include <sys/select.h>
#include <assert.h>
extern sharedMemory *sm;


int readOpponent(char *buffer) {
    sscanf(buffer, "+ %d %s", &(sm->enemy.playerNr), buffer);
    sm->enemy.registered = *(buffer + strlen(buffer) - 1);
    strncat(sm->enemy.playerName, buffer, strlen(buffer) - 3);
    return 0;
}

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
int tokenizeshit(char* buffer, char* token[100]){
    int i  = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
        //printf("Dia chi cua token o vi tri i %d\n",i);
        //printf("in tokenistshit :%s\n",token[i]);
        
    }
    return i;
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

int performConnection(int socketfd, char *gameId, int playerNr, int fd[2]) {
    size_t size =512;
    char array[size];
    char *buffer;
    //char *buffer = malloc(size);
    char *token[100];
    /* for(int j= 0; j<=20;j++ ){
        token[j]= malloc(size);
    } */
    
    int numberofToken;
    int tokenIndex;
    //char* line= malloc(size);
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

            //printf("inbuffer: %s\n", buffer);
            // Get data
            //
            //printf("This is in buffer: %s",buffer);
            numberofToken= tokenizeshit(buffer,token);
            for (tokenIndex=0; tokenIndex<numberofToken; tokenIndex++) {
                buffer = token[tokenIndex] ;
                //printf("%d\n", tokenIndex );
                //printf("S:%s\n",token[tokenIndex]);
                
                //printf("Dia chi con tro khi doi tang tokenindex %d\n", *buffer);

            switch(buffer[0]){
                case '+':
                    if(strcmp(buffer,"+ MNM Gameserver v2.3 accepting connections") == 0){
                        printf("S:%s\n", buffer);
                        toServer("VERSION 2.3\n");
                        //printf("Dia chi con tro %d\n", *buffer);
                        //bzero(buffer, size);
                        
                    }
                    else if(strcmp(buffer,"+ Client version accepted - please send Game-ID to join") == 0){
                        printf("S:%s\n", buffer);
                        threeServer("ID ", gameId, "\n");
                        //bzero(buffer, size);
                        
                    }
                    else if(strcmp(buffer,"+ PLAYING Reversi") == 0){
                        printf("S:%s\n", buffer);
                        //printf("Dia chi con tro %d\n", *buffer);
                        //bzero(buffer, size);
                        
                    }
                    else if(strncmp(buffer,"+ Game from",11) == 0){
                        printf("S:%s\n", buffer);
                        if(playerNr == 1 || playerNr ==2 ){
                                toServer("PLAYER\n");
                        }
                        //printf("Dia chi con tro %d\n", *buffer);    
                        //bzero(buffer, size);
                        
                    }
                    else if(strncmp(buffer,"+ YOU",5) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ YOU %d %s",&(sm->me.playerNr), sm->me.playerName)!= 2){
                            exit(EXIT_FAILURE);
                        }
                        //printf("Dia chi con tro %d\n", *buffer);
                        //bzero(buffer, size);

                    }
                    else if(strncmp(buffer,"+ TOTAL",7) == 0){
                        printf("S:%s\n", buffer);
                            /*
                        if(sm->me.playerNr == 0){
                        readField(sm,buffer,token,4);
                        toServer("THINKING\n");
                        }
                        */
                        //printf("Dia chi con tro %d\n", *buffer);
                        //bzero(buffer,size);
                        
                    }/* 
                    else if(strncmp(buffer, "+ 1 %s")) {

                    }
                    else if(strncmp(buffer,"+ 1 \"White\" ",12) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ %d %s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                            return false;
                        }
                        //bzero(buffer,size);
                    }
                    else if(strncmp(buffer,"+ 0 White ",10) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ %d %s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                            return false;
                        }
                        //bzero(buffer,size);
                        
                    }
                    else if(strncmp(buffer,"+ 1 Black ",10) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ %d %s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                            return false;
                        }
                        //bzero(buffer,size);
                        
                    }
                    else if(strncmp(buffer,"+ 0 \"Black\" ",12) == 0){
                        printf("S:%s\n", buffer);
                        if(sscanf(buffer,"+ %d %s %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                            return false;
                        }
                        //bzero(buffer,size);
                        
                    } */
                    else if(strcmp(buffer,"+ ENDPLAYERS") == 0){
                        printf("S:%s\n", buffer);
                        //bzero(buffer,size);
                        
                    }
                    else if(strcmp(buffer,"+ WAIT") == 0){
                        printf("S:%s\n", buffer);
                        toServer("OKWAIT\n");
                        bzero(buffer,size);
                        
                    }
                    else if(strcmp(buffer,"+ MOVE 3000") == 0){ 
                        printf("S:%s\n", buffer);
                        /*
                        tokenizeshit(buffer,token);
                        readField(sm,buffer,token,1);
                        toServer("THINKING\n");
                        */ 
                        //bzero(buffer,size);
                        
                    }
                    /*
                    if(strncmp(buffer,"+ MOVE 3000",11) == 0){ 
                        
                        tokenizeshit(buffer,token);
                        readField(sm,buffer,token,1);
                        toServer("THINKING\n");
                        bzero(buffer,size);
                        break;
                    }
                    */
                    else if(strcmp(buffer,"+ FIELD 8,8") == 0){ 
                        printf("S:%s\n", buffer);
                        sm->spielFeld.width = 8;
                        sm->spielFeld.height = 8;
                        //tokenizeshit(buffer,token);
                        //readField(sm,buffer,token,tokenIndex+1);
                        //tokenIndex++;
                        /*
                        for(int row = 7;row >= 0;row--){
                            tokenIndex++ ;
                            buffer = token[tokenIndex];
                            printf("S:%s\n", buffer);
                            sscanf(buffer,"+ %*d %c %c %c %c %c %c %c %c",
                            &(sm->spielFeld.Feld[row][0]),&(sm->spielFeld.Feld[row][1]),
                            &(sm->spielFeld.Feld[row][2]),&(sm->spielFeld.Feld[row][3]),
                            &(sm->spielFeld.Feld[row][4]),&(sm->spielFeld.Feld[row][5]),
                            &(sm->spielFeld.Feld[row][6]),&(sm->spielFeld.Feld[row][7]));
                            
                        }
                        */
                            //toServer("THINKING\n");
                        //bzero(buffer,size);
                        //printf("tokenIndex ist:%d\n", tokenIndex);
                        
                    }
                    else if(isdigit(buffer[2]) && (buffer[4] == 'W' || buffer[4] == 'B' || buffer[4] == '*')) {
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
                    else if(isdigit(buffer[2])) {
                        printf("S: %s\n", buffer);
                        readOpponent(buffer);
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
                        /*
                        char themove[3];
                        read(fd[0],themove, sizeof(themove));
                        threeServer("PLAY ",themove,"\n");
                        */
                        //bzero(buffer,size);
                        
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
                    /*
                    if(strncmp(buffer,"+",1) == 0){
                        printf("S:%s qưqưqư\n", buffer);
                        break;
                    }
                    */
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
   //free(buffer);
    exit(EXIT_SUCCESS); 
 }
