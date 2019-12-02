#include "signalHandler.h"

void handler(int sig) {
    if(sig == SIGUSR1) {
        think();
    }
}
