#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

// Prolog Phase der Kommunikation
// Zuweisung eines freien Spielers durch Gameserver
// @param int socketfd: FIle-Deskriptor des Sockets
bool performConnection(int socketfd, char *gameId, int playerNr);
