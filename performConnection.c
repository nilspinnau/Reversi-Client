#include "performConnection.h"


// vielleicht noch fehlerbehandlung von send bearbeiten
int performConnection(int socketfd, char *gameId, int playerNr) {

    char *version = "2.3";

    char data[256];

    while(recv(socketfd, data, sizeof(data), 0) != 0) {
        switch(data[0]) {
            case '+':
                printf("S: %s", data);

                if(strstr(data, "+ MNM Gameserver") != NULL) {
                    // sending client id/version
                    char answer[30];
                    sprintf(answer, "VERSION %s%s", version, "\n\0");
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    
                } else if(strstr(data, "+ Client version accepted - please send Game-ID to join") != NULL) {
                    char id[30];
                    sprintf(id, "ID %s%s", gameId, "\n\0");
                    send(socketfd, id, sizeof(id), 0);
                    printf("C: %s", id);

                } else if(strstr(data, "+ PLAYING") != NULL) {
                    // nothing happens, server sends another message
                
                } else if(strstr(data, "+ GAMENAME") != NULL) {
                    char player[30];
                    sprintf(player, "PLAYER %i%s", playerNr, "\n\0");
                    send(socketfd, player, sizeof(player), 0);
                    printf("C: %s\n", player);
                
                } else if(strstr(data, "+ YOU") != NULL) {
                    // server: Mitspielernummer and Mitspielername
                
                } else if(strstr(data, "+ TOTAL") != NULL) {
                    // server: Mitspieleranzahl
                
                } else if(strstr(data, "+ ENDPLAYERS") != NULL) {
                    exit(EXIT_SUCCESS);
                }
                break;       
            default:
                // disconnect, error message from server
                printf("S: %s", data);            
                close(socketfd);
                exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

