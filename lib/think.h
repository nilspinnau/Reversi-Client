#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#include "../lib/handler.h"
#include "../lib/position.h"
#include "../lib/signalHandler.h"
#include <pthread.h>
#include <sys/time.h>
#endif

int think(sharedMemory *sm);

void *findSolution(void *arg);
