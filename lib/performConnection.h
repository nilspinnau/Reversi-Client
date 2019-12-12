#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "../lib/handler.h"

// Prolog Phase der Kommunikation
// Zuweisung eines freien Spielers durch Gameserver
// @param int socketfd: FIle-Deskriptor des Sockets
// @param gameId: Id des Reversi Spiels, welches erstellt wurde
// @param playerNr: preferierte Spielnummer, wenn nicht frei, dann zuweisung von Server
bool performConnection(int socketfd, char *gameId, int playerNr);
