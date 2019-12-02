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

                if(strstr(buffer, "+ MNM Gameserver") != NULL) {
                    // sending client id/version
		    
                    bzero(buffer, sizeof(buffer));
                    sprintf(buffer, "VERSION %s%s", version, "\n\0");
                    char answer[strlen(buffer)];
		            strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    bzero((char *) &buffer, sizeof(buffer));
                    bzero(answer, sizeof(answer));
                
                } else if(strstr(buffer, "+ Client version accepted - please send Game-ID to join") != NULL) {
					// sending GameId

                    bzero(buffer, sizeof(buffer));
                    sprintf(buffer, "ID %s%s", gameId, "\n\0");
                    char answer[strlen(buffer)];
		    		strncpy(answer,buffer,strlen(buffer));
                    send(socketfd, answer, sizeof(answer), 0);
                    printf("C: %s", answer);
                    bzero((char *) &buffer, sizeof(buffer));
                    bzero(answer, sizeof(answer));
                
                } else if(strstr(buffer, "+ PLAYING") != NULL) {

                    bzero(buffer, sizeof(buffer));
		    		read(socketfd,buffer, sizeof(buffer));
                    int length = strlen(buffer);
		    		printf("S: %.*s", length, buffer);
	 	    		send(socketfd,"PLAYER\n\0",sizeof(char)*8,0);
					printf("%d",playerNr);
                    bzero((char *) &buffer, sizeof(buffer));

                }  else if(strstr(buffer, "+ YOU") != NULL) {
                    // server: Mitspielernummer and Mitspielername
                    
                    bzero((char *) &buffer, sizeof(buffer));
                
                } else if(strstr(buffer, "+ TOTAL") != NULL) {
                    // server: Mitspieleranzahl
                    
		    		printf("S: %s", buffer);
                    bzero((char *) &buffer, sizeof(buffer));
                
                } else if(strstr(buffer, "+ ENDPLAYERS") != NULL) {
					
                    readField(socketfd);
                    exit(EXIT_SUCCESS);
                
                } else {
					// disconnect, error message from server
					printf("S: %s", buffer);            
					close(socketfd);
					exit(EXIT_FAILURE);
				}
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

int game(int socketfd) {
    char buffer[256] = {0};
    while (recv(socketfd,buffer,sizeof(buffer),0) != 0) {
        if (strstr(buffer, "+ ENDFIELD") != NULL) {
            send(socketfd,"THINKING\n\0",10*sizeof(char),0);
            bzero((char *) &buffer, sizeof(buffer));
        }
        else if (strstr(buffer, "+ WAIT") != NULL) {
			send(socketfd,"OKWAIT\n\0",8*sizeof(char),0);
            bzero((char *) &buffer, sizeof(buffer));
        }
        else if (strstr(buffer, "+ MOVE %d") != NULL) {
            //int time = strtod(buffer);
            read(socketfd, buffer, sizeof(buffer));
        }
        else if (strstr(buffer, "+ GAMEOVER") != NULL) {
            
        }
    }
    return 0;
}

int readField(int socketfd) {
	char buffer[256] = {0};
    size_t size = sizeof(buffer);
	while (recv(socketfd,buffer,sizeof(buffer),0) != 0) {
		if (strstr(buffer, "+ FIELD 8,8") != NULL) {
			for(int i=0; i < 8; i++) {
                read(socketfd, buffer, size);
                bzero(buffer, size);
            }
            break;
        }
	}
    read(socketfd, buffer, size);
    if(strstr(buffer, "+ ENDFIELD")) {
        write(socketfd, "THINKING\n\0", 10*sizeof(char));
    }
    // anstoß des thinkers per SIGUSR1
    int *shm_ptr = initialize();
    printf("%p", shm_ptr);
    //kill(shm->thinker, SIGUSR1);
	return 0;
}

