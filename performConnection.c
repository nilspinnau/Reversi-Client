#include "performConnection.h"


// vielleicht noch fehlerbehandlung von send bearbeiten
int performConnection(int socketfd, char *gameId, int playerNr) {

    char *version = "2.3";
    char buffer[256];
    char data[256];

    while(recv(socketfd, data, sizeof(data), 0) != 0) {
        switch(data[0]) {
            case '+':
                printf("S: %s", data);
		

                if(strstr(data, "+ MNM Gameserver") != NULL) {
                    // sending client id/version
		    
                    sprintf(buffer, "VERSION %s%s", version, "\n\0");
                    char answer[strlen(buffer)];
		    strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    
                } else if(strstr(data, "+ Client version accepted - please send Game-ID to join") != NULL) {
					// sending GameId

                    sprintf(buffer, "ID %s%s", gameId, "\n\0");
                    char answer[strlen(buffer)];
		    		strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);

                } else if(strstr(data, "+ PLAYING") != NULL) {

		    		read(socketfd,buffer, sizeof(buffer));
		    		printf("S: %s", buffer);
	 	    		send(socketfd,"PLAYER\n\0",sizeof(char)*8,0);
					printf("%d",playerNr);
                    // nothing happens, server sends another message
                
                }  else if(strstr(data, "+ YOU") != NULL) {
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

