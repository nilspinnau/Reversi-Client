#include "../lib/think.h"
#include "../lib/helpFunction.h"

extern int fd[2];
move pos[60];


int think(sharedMemory *sm){ 
    /* int tempX=0;
    int tempY=0;
    int* x=&tempX;
    int* y=&tempY;
 */
    // after 2 seconds stop searching 
    alarm(2);

    char whoseTurn = {0};
	int numMoves = 0;
    if((sm->me.playerNr) == 0) whoseTurn = 'B';
    else whoseTurn = 'W';

    for(int i = 0; i < 60; i++) {
        pos[i].x = 0;
        pos[i].y = 0;
        pos[i].val = 0;
    }
    
	numMoves = getMoveList(sm->spielFeld.Feld, pos, whoseTurn);
    printf("Number of moves: %d\n", numMoves);    


    // creating threads
    pthread_t p[numMoves];
    for(int i = 0; i < numMoves; i++) {
        
        threadArguments *args = malloc(sizeof(threadArguments));
        copyBoard(sm->spielFeld.Feld, args->feld);
        args->i = i;
        args->whoseTurn = whoseTurn;
        args->numMoves = numMoves;
        printf("x in thread %d: %d\n", i, pos[i].x);
        printf("y in thread %d: %d\n", i, pos[i].y);

        pthread_create(&p[i], NULL, findSolution, args);
    }
    for(int i = 0; i < numMoves; i++) {
        pthread_join(p[i], NULL);
    }
    int bestX, bestY = 0;
    int temp = pos[0].val;
    int bestIndex = 0;

    for(int i = 0; i < numMoves; i++) {
        printf("Wert x an index: %d %d\n", i, pos[i].x);
        printf("Wert y an index: %d %d\n", i, pos[i].y);
    }
    for(int i = 1; i < numMoves; i++) {
        // check which is the best solution
        if(pos[i].val > temp) {
            temp = pos[i].val;
            bestIndex = i;
        }
    }
    bestX = pos[bestIndex].x;
    bestY = pos[bestIndex].y;
    printf("Best x: %d and best y: %d\n", bestX, bestY);
    write(fd[1], position(bestX, bestY), 3*sizeof(char));

    return 0;    
}

void *findSolution(void *args) {
    minimaxDecision(args);
    return pos;
}
  