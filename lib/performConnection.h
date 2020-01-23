#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#include "../lib/handler.h"
#include "../lib/readField.h"
#endif

// Prolog Phase der Kommunikation
// Zuweisung eines freien Spielers durch Gameserver
// @param int socketfd: FIle-Deskriptor des Sockets
// @param gameId: Id des Reversi Spiels, welches erstellt wurde
// @param playerNr: preferierte Spielnummer, wenn nicht frei, dann zuweisung von Server
int performConnection(int socketfd, char *gameId, int playerNr,int fd[2]);


// method to read the playerName into the sharedMemory, aswell as him being registered and his playerNr
// this method doesnt need sscanf, so reducing the error suspectibility
int readOpponent(char *buffer);
