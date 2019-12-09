#include "../lib/performConnection.h"

// vielleicht noch fehlerbehandlung von send bearbeiten
int performConnection(int socketfd, char *gameId, int playerNr) {

    char *version = "2.3";
    char buffer[256] = {0};
    int n = 0;

    while(recv(socketfd, buffer, sizeof(buffer), 0) != 0) {
        switch(buffer[0]) {
            case '+':
                n = strlen(buffer);
                printf("S: %.*s", n, buffer);

                if(strstr(buffer, "+ MNM Gameserver")) {
                    // sending client id/version
		    
                    bzero(buffer, sizeof(buffer));
                    sprintf(buffer, "VERSION %s%s", version, "\n\0");
                    char answer[strlen(buffer)];
		            strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    bzero(buffer, sizeof(buffer));
                    bzero(answer, sizeof(answer));
                } else if(strstr(buffer, "+ Client version accepted - please send Game-ID to join") != NULL) {
					// sending GameId

                    bzero(buffer, sizeof(buffer));
                    sprintf(buffer, "ID %s%s", gameId, "\n\0");
                    char answer[strlen(buffer)];
		    		strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    bzero(buffer, sizeof(buffer));
                    bzero(answer, sizeof(answer));
                
                } else if(strstr(buffer, "+ PLAYING")) {

                    bzero(buffer, sizeof(buffer));
		    		read(socketfd,buffer, sizeof(buffer));
                    int length = strlen(buffer);
		    		printf("S: %.*s", length, buffer);
	 	    		send(socketfd,"PLAYER\n",sizeof(char)*7,0);
					printf("%d",playerNr);
                    bzero(buffer, sizeof(buffer));

                } else if(strstr(buffer, "+ ENDPLAYERS")) {
                    exit(EXIT_SUCCESS);
                } 
                bzero(buffer, sizeof(buffer));
                break;       
            default:
                // disconnect, error message from server
                printf("S: %s", buffer);            
                close(socketfd);
                exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
