#include "../lib/signalHandler.h"
#include "../lib/think.h"
extern sharedMemory *sm;
extern int fd[2];
extern int searchPly;


void signalhandler(int signum) {
    if(signum == SIGUSR1 && sm->thinkFlag) {
        think(sm);
        sm->thinkFlag = false;
    }

}

void signalAlarm(int signum) {
    if(signum == SIGALRM) {
        printf("Alarm wurde gelesen\n");
        searchPly = MAX;
        printf("SearchPly ist jt: %d\n", searchPly);
    }
}