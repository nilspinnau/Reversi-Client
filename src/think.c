#include "../lib/think.h"
#include "../lib/helpFunction.h"

extern int fd[2];


int think(sharedMemory *sm){ 
    /* int tempX=0;
    int tempY=0;
    int* x=&tempX;
    int* y=&tempY;
 */
    // after 2 seconds stop searching 
    alarm(2);

    // initialize struct with every needed argument
    threadArguments *args;
    args->feld = sm->spielFeld.Feld;

    int moveX[60], moveY[60];
	int numMoves=0;
    if((sm->me.playerNr)==0) args->whoseTurn = 'B';
    else args->whoseTurn = 'W';

	numMoves=getMoveList(sm->spielFeld.Feld, moveX, moveY, numMoves, args->whoseTurn);
    

    // creating threads
    pthread_t p[numMoves];
    for(int i = 0; i < numMoves; i++) {
        // args.i = i; doesnt work obv
        pthread_create(&p[i], NULL, findSolution, args);
    }
    int bestX = 0;
    int bestY = 0;
    int bestIndex = 0;
    for(int i = 0; i < numMoves-1; i++) {
        // check which is the best solution
        if(args->pos[i].val > args->pos[i+1].val) {
            bestIndex = i;
            bestX = args->pos[i].x;
            bestY = args->pos[i].y;
        }
    }
    write(fd[1], position(&bestX, &bestY), 3*sizeof(char));

    for(int i = 0; i < numMoves; i++) {
        pthread_join(&p[i], NULL);
    }
    return 0;    
}

void *findSolution(void *arg) {
    threadArguments *args = arg;
    minimaxDecision(args->feld,args->whoseTurn,args);
    return arg;
}
  