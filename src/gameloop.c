#include "../lib/gameloop.h"
#include "../lib/handler.h"
#include <sys/select.h>
#include <assert.h>
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


bool gameloop(sharedMemory* sm, int fd[2]){
    
    char *loopbuffer;
    loopbuffer = getbuffer();
    if(sscanf(loopbuffer,"+ YOU %d %*s player\n",&(sm->myPlayerNr))!= 1)return false;// not enemy + set enemy to oppposite yours
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
    
    while(!exit){
     /*
        fd_set readSet;
        int max_fd= -1;
        FD_ZERO(&readSet);
        safe_fdSet(fd[0],&readSet,&max_fd);
        safe_fdSet(sockfd,&readSet,&max_fd);
    // note the max_fd+1
    if(select(max_fd+1,&readSet,NULL,NULL,NULL)<=0){
            perror("Select\n");
    }else{
    if (FD_ISSET(sockfd, &readSet)){ 
        */
        if(strcmp(loopbuffer,"+ WAIT\n") == 0){
            toServer("OKWAIT\n");
        }
        if(strcmp(loopbuffer,"+ GAMEOVER\n") == 0){
            readField(sm,loopbuffer);
            exit= true;
            //handle who is winner
            break;
        }
        if(strcmp(loopbuffer,"- TIMEOUT Be faster next time\n") == 0){
            printf("Timeout -> Exiting Programm\n");
            break;
        }
        if(strcmp(loopbuffer,"+ MOVE 3000") == 0){ //nur beim 1. mal -Teil von größerem String
            loopbuffer = nextbufLine();
            if(!readField(sm,loopbuffer)){
                printf("Field could not be read\n");
                break;
            }
            toServer("THINKING\n");
            if(!isnext("+ OKTHINK\n")){
                printf("THINKING NOT ALLOWED\n");
                break;
            }
            //thinker anstoßen
            sm->thinker = getppid();
            kill(sm->thinker,SIGUSR1);
            char themove[3];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move\n");
                break;
            }
        }
        if(strcmp(loopbuffer,"+ MOVE 3000\n") == 0){ //nur beim 1. mal -Teil von größerem String
            getLine();
            resetLinebuf();
            loopbuffer = nextbufLine();
            if(!readField(sm,loopbuffer)){
                printf("Field could not be read\n");
                break;
            }
            toServer("THINKING\n");
            if(!isnext("+ OKTHINK\n")){
                printf("THINKING NOT ALLOWED\n");
                break;
            }
            //thinker anstoßen
            sm->thinker = getppid();
            kill(sm->thinker,SIGUSR1);
            char themove[3];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move\n");
                break;
            }
        }
       //resetLinebuf();
       loopbuffer = getLine();     
     /*
    }
  
        if(FD_ISSET(fd[0], &readSet)){
            char themove[2];
            read(fd[0],themove, sizeof(themove));
            threeServer("PLAY ",themove,"\n");
            }   
        }
     */   
    }
    return exit;
}
