#include "../lib/performConnection.h"
#include <time.h>

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

                } else if(strstr(buffer, "+ ENDFIELD")) {
                    write(socketfd, "THINKING\n", 9*sizeof(char));
                    printf("C: THINKING\n");
                    bzero(buffer, sizeof(buffer));
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
    readField(socketfd);
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
            //timer to measure maximum length of a turn
            char *ptr;
            int msec = 0, time = strtod(buffer,&ptr);
            clock_t before = clock();
            int iterations = 0;
            do {
                clock_t difference = clock() - before;
                msec = difference * 1000 / CLOCKS_PER_SEC;
                iterations++;
            } while (msec < time);
            if (msec == time) {
                exit(EXIT_FAILURE);
            }
            read(socketfd, buffer, sizeof(buffer));
            //print out field state of given line and column
            if (strstr(buffer, "+ ENDFIELD")) {
                    write(socketfd, "THINKING\n", 9*sizeof(char));
                    bzero(buffer, sizeof(buffer));
            }
            else if (strstr(buffer, "+ FIELD %d %d") != NULL) {
                char *ptr;
                int line = strtod(buffer,&ptr); 
                int column = strtod(ptr,&ptr);
                for (int i=0; i < line; i++) {
                    for (int i=0; i < column; i++) {
                        read(socketfd, buffer, sizeof(buffer));
                        bzero(buffer, sizeof(buffer));
                    }
                }
            }
        }
        else if (strstr(buffer, "+ GAMEOVER") != NULL) {
            read(socketfd, buffer, sizeof(buffer));
            bzero(buffer, sizeof(buffer));
            if (strstr(buffer, "+ FIELD %d %d") != NULL) {
                char *ptr;
                int line = strtod(buffer,&ptr); 
                int column = strtod(ptr,&ptr);
                for (int i=0; i < line; i++) {
                    for (int i=0; i < column; i++) {
                        read(socketfd, buffer, sizeof(buffer));
                        bzero(buffer, sizeof(buffer));
                    }
                }
                if (strstr(buffer, "+ ENDFIELD") != NULL) {
                    read(socketfd, buffer, sizeof(buffer));
                    bzero(buffer, sizeof(buffer));
                    if (strstr(buffer, "+ QUIT") != NULL) {
                        close(socketfd);
                    }
                }
            }
        }
    }
    return 0;
}



int readField(int socketfd) {
	char buffer[256] = {0};
    size_t size = sizeof(buffer);
	while (recv(socketfd,buffer,sizeof(buffer),0) != 0) {
		if(strstr(buffer, "+ ENDFIELD")) {
                    write(socketfd, "THINKING\n", 9*sizeof(char));
                    bzero(buffer, sizeof(buffer));
        } else if (strstr(buffer, "+ FIELD 8,8") != NULL) {
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
    //kill(shm->thinker, SIGUSR1);
	return 0;
}
