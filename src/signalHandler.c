#include "../lib/signalHandler.h"
#include "../lib/think.h"
extern sharedMemory *sm;
extern int fd[2];
void signalhandler(int signum) {
    if(signum == SIGUSR1) {
        sm->thinkFlag = true;    
    }

}