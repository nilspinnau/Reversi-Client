#include "../lib/performConnection.h"
#include "../lib/handler.h"


// vielleicht noch fehlerbehandlung von send bearbeiten
bool performConnection(int socketfd, char *gameId, int playerNr) {

    setSocket(socketfd);
    if(isnext("+ MNM Gameserver v2.3 accepting connections\n")){
        toServer("VERSION 2.3\n");
        if(isnext("+ Client version accepted - please send Game-ID to join\n")){
            threeServer("ID ", gameId, "\n");
            if(isnext("+ PLAYING Reversi\n")){
                getLine();
                if(playerNr == 1) toServer("PLAYER\n");
                else toServer("PLAYER\n");
                if(isnext("- No free player\n")){
                    printf("No Playerspot availiable\n");
                    return false;
                }
                
            }
            else{
                printf("Game ID not Accepted\n");
                return false;
            }
        }
        else{
            printf("Version not Accepted\n");
            return false;
        }
    }
    else{
        printf("Server not responding\n");
        return false;
    }

    return true;
}
