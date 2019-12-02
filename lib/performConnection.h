#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "../lib/signalHandler.h"
#include "../lib/shm.h"


// Prolog Phase der Kommunikation
// Zuweisung eines freien Spielers durch Gameserver
// @param int socketfd: FIle-Deskriptor des Sockets
int performConnection(int socketfd, char *gameId, int playerNr);
int readField(int socketfd);
int game(int socketfd);
