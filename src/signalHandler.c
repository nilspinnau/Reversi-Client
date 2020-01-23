#include "../lib/signalHandler.h"
#include "../lib/think.h"
extern sharedMemory *sm;

void signalhandler(int signum) {
    sm->thinkFlag = false;
    if(signum == SIGUSR1) {
        sm->thinkFlag = true;
        think(sm);
    }
}