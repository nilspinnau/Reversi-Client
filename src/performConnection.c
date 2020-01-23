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
int tokenizeshit(char* buffer, char* token[100]){
    int i  = 0;
    //char *token[256];
    token[0]=strtok(buffer, "\n");
    while((token[i]) !=NULL){
        i++ ;
        token[i]= strtok(NULL,"\n");
        //printf("Dia chi cua token o vi tri i %d\n",i);
        printf("in tokenistshit :%s\n",token[i]);
        
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

bool performConnection(int socketfd, char *gameId, int playerNr, int fd[2]) {
    size_t size =sizeof(char)*256;
    char* buffer = (char*)malloc(size);
    char *token[100];
    
    for(int j= 0; j<=20;j++ ){
        token[j]= malloc(size);
    }
    
    int numberofToken;
    int tokenIndex;
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
        
        bzero(buffer, size);
        read(socketfd, buffer,size);
        // Get data
        //
        printf("This is in buffer: %s",buffer);
        numberofToken= tokenizeshit(buffer,token);
        for (tokenIndex=0; tokenIndex<numberofToken; tokenIndex++) {
            buffer= token[tokenIndex] ;
            //printf("%d\n", tokenIndex );
            //printf("S:%s\n",token[tokenIndex]);
            
            //printf("Dia chi con tro khi doi tang tokenindex %d\n", *buffer);

        switch(buffer[0]){
            case '+':
                if(strcmp(buffer,"+ MNM Gameserver v2.3 accepting connections") == 0){
                    printf("S:%s\n", buffer);
                    toServer("VERSION 2.3\n");
<<<<<<< HEAD
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
                    
=======
                   //printf("Dia chi con tro %d\n", *buffer);
                    //bzero(buffer, size);
                    break;
                }
                if(strcmp(buffer,"+ Client version accepted - please send Game-ID to join") == 0){
                    printf("S:%s\n", buffer);
                    threeServer("ID ", gameId, "\n");
                    //bzero(buffer, size);
                    break;
                }
                if(strcmp(buffer,"+ PLAYING Reversi") == 0){
                    printf("S:%s\n", buffer);
                    //printf("Dia chi con tro %d\n", *buffer);
                    //bzero(buffer, size);
                    break;
                }
                if(strncmp(buffer,"+ Game from",11) == 0){
                    printf("S:%s\n", buffer);
                    if(playerNr == 1 || playerNr ==2 ){
                         toServer("PLAYER\n");
                    }
                    //printf("Dia chi con tro %d\n", *buffer);    
                    //bzero(buffer, size);
                    break;
                }
                if(strncmp(buffer,"+ YOU",5) == 0){
                    printf("S:%s\n", buffer);
                    if(sscanf(buffer,"+ YOU %d %s player",&(sm->me.playerNr), sm->me.playerName)!= 2){
                        return false;
                    }
                    //printf("Dia chi con tro %d\n", *buffer);
                    //bzero(buffer, size);

                    break;
                }
                if(strncmp(buffer,"+ TOTAL",7) == 0){
                    printf("S:%s\n", buffer);
                     /*
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
                    if(sm->me.playerNr == 0){
                    readField(sm,buffer,token,4);
                    toServer("THINKING\n");
                    }
<<<<<<< HEAD
                    
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
=======
                    */
                    //printf("Dia chi con tro %d\n", *buffer);
                    //bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ 1 \"White\" ",12) == 0){
                    printf("S:%s\n", buffer);
                    if(sscanf(buffer,"+ %d %s player %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                        return false;
                    }
                    //bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ 0 \"Black\" ",12) == 0){
                    printf("S:%s\n", buffer);
                    if(sscanf(buffer,"+ %d %s player %d",&(sm->enemy.playerNr),sm->enemy.playerName,&(sm->me.registered))!= 3){
                        return false;
                    }
                    //bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ ENDPLAYERS") == 0){
                    printf("S:%s\n", buffer);
                    //bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ WAIT") == 0){
                    printf("S:%s\n", buffer);
                    toServer("OKWAIT\n");
                    bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ MOVE 3000") == 0){ 
                    printf("S:%s\n", buffer);
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
                 /*
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,1);
                    toServer("THINKING\n");
                  */ 
<<<<<<< HEAD
                    bzero(buffer,size);
                    break;
                }
=======
                    //bzero(buffer,size);
                    break;
                }
                /*
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
                if(strncmp(buffer,"+ MOVE 3000",11) == 0){ 
                 
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,1);
                    toServer("THINKING\n");
                    bzero(buffer,size);
                    break;
                }
<<<<<<< HEAD

                if(strncmp(buffer,"+ FIELD 8,8", 11) == 0){ 
                    tokenizeshit(buffer,token);
                    readField(sm,buffer,token,0);
                    toServer("THINKING\n");
                    bzero(buffer,size);
                    break;
                }
                        
                if(strcmp(buffer,"+ OKTHINK\n")== 0){
=======
                */
                if(strcmp(buffer,"+ FIELD 8,8") == 0){ 
                    printf("S:%s\n", buffer);
                    //tokenizeshit(buffer,token);
                    //readField(sm,buffer,token,tokenIndex+1);
                    //tokenIndex++;
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
                     //toServer("THINKING\n");
                    //bzero(buffer,size);
                    //printf("tokenIndex ist:%d\n", tokenIndex);
                    break;
                }
                if(strcmp(buffer,"+ ENDFIELD") == 0){
                    printf("S:%s\n", buffer);
                    //bzero(buffer,size);
                    if(!Exit){
                    toServer("THINKING\n");
                    }
                    break;
                }

                if(strcmp(buffer,"+ OKTHINK")== 0){
                    printf("S:%s\n", buffer);
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
                    sm->thinker = getppid();
                    kill(sm->thinker,SIGUSR1);
                    /*
                    char themove[3];
                    read(fd[0],themove, sizeof(themove));
                    threeServer("PLAY ",themove,"\n");
                    */
<<<<<<< HEAD
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

=======
                    //bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ MOVEOK") == 0){
                    printf("S:%s\n", buffer);
                    //bzero(buffer,size);
                    break;
                }
                if(strcmp(buffer,"+ GAMEOVER")==0){
                    printf("S:%s\n", buffer);
                    Exit=true;
                    //bzero(buffer,size);
                    break;
                }
                if(strncmp(buffer,"+ PLAYER0WON",12) == 0){
                    printf("S:%s\n", buffer);
                    break;
                }
                if(strncmp(buffer,"+",1) == 0){
                    printf("S:%s qưqưqư\n", buffer);
                    break;
                }
                if(strncmp(buffer,"+ PLAYER1WON",12) == 0){
                    printf("S:%s\n", buffer);
                    break;
                }
                if(strcmp(buffer,"+ QUIT")==0){
                    printf("S:%s\n", buffer);
                    exit(EXIT_SUCCESS);
                    return Exit;
                }
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
            case '-':
                if(strcmp(buffer,"- TIMEOUT Be faster next time") == 0){
                    printf("S:%s\n", buffer);
                    printf("Timeout -> Exiting Programm\n");
                    exit(EXIT_FAILURE);
                    return Exit;
                }    
                if(strcmp(buffer,"- No free player") == 0){
                    printf("S:%s\n", buffer);
                    printf("No Player available\n");
                    exit(EXIT_FAILURE);
                    return Exit;
                }
                if(strcmp(buffer,"- Did not expect response from client") == 0){
                    printf("S:%s\n", buffer);
                    printf("Wrong response\n");
                    exit(EXIT_FAILURE);
                    return Exit;
                }
                if(strcmp(buffer,"- Invalid Move: Invalid position") == 0){
                    printf("S:%s\n", buffer);
                    printf("Wrong Move\n");
                    exit(EXIT_FAILURE);
                    return Exit;
                }
                if(strcmp(buffer,"- Internal error. Sorry & Bye\n") == 0){
                    printf("S:%s\n", buffer);
                    printf("Server malfunction\n");
                    return Exit;
                }    
            default:
                return false;
            }
<<<<<<< HEAD
            
=======
        }
>>>>>>> ef8015269107bf15322e68f02c03be98e70da2b5
        }
        if(FD_ISSET(fd[0], &readSet)){
            char themove[3];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            }  
    }    
    //free(buffer);
    return Exit; 
}
