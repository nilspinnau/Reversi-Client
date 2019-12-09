#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>

#include "../lib/signalHandler.h"
#include "../lib/performConnection.h"
#include "../lib/think.h"
#include "../lib/getconfig.h"
#include "../lib/struct_H.h"



/*
#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
*/

int main(int argc, char **argv) {

    int opt;
    //char *gameId;
    char gameId[14];
    int playerNr;

    char path[256];
    strcpy(path,"config.conf");
    configs res;
    configs* rp;
    memset(&res,0,sizeof(configs));

    while ((opt = getopt (argc, argv, "g:p:f:")) != -1) {
        switch (opt) {
            case 'g':
                if (strlen(optarg) != 13) {
					perror("Bitte 13-stellige Game-Id eingeben");
					exit(EXIT_FAILURE);
                }
                else {
                strcpy(gameId, optarg);
                } 
                break;
            case 'p':
                playerNr = atoi(optarg);
                if (playerNr < 1 || playerNr > 2) {
                	perror("Spieleranzahl 1 oder 2");
					exit(EXIT_FAILURE);
				}
                break;
            case 'f':
                bzero((char*) &path, sizeof(path));
                strcpy(path,optarg);
                 
                break;    
        }
    }
   
    rp = getconfig(&res,path);
    if(rp == NULL){
        printf("configfile err");
        return 0;
    }
    printf("%s\n",rp->game_kind);
    printf("%s\n",rp->host_name);
    printf("%d\n",rp->port_nr);
   
    int sockfd;
    //portno
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char* ) &serv_addr, sizeof(serv_addr));
    //portno = PORTNUMBER;

    struct hostent *server;
    server = gethostbyname(rp->host_name);
    if (server == NULL) {
        perror("ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(rp->port_nr);
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
    }
    int fd[2];
    pid_t pid =0;
    int ret_code =0;
    fd[0]=fd[1]=0;
    struct sharedMemory* sm = malloc(sizeof(struct sharedMemory));
    int shm_id = shmget(IPC_PRIVATE,sizeof(struct sharedMemory),0);
    sm = (struct sharedMemory*) shmat(shm_id,NULL,0);
    
    pid = fork();
    if (pid < 0) {
        perror ("Fehler bei fork().");
        exit(EXIT_FAILURE);
    }
   /*
   * THINKER = ELTERNPROZESS
   */
    if(pid >0){
        // READSEITE der Pipe schliessen
        close(fd[0]);
        sm->connector = getppid();
        ret_code = waitpid(pid, NULL, 0);
        signal(SIGUSR1, handler);
        if (ret_code < 0) {
            perror ("Fehler beim Warten auf Kindprozess.");
            exit(EXIT_FAILURE);
        }  
        char *answer = think();
        write(fd[1], answer, sizeof(answer));
    }
    
    /*
    * Connector = Kindprozess
    */
    else {
        // Schreibseite der Pipe schliessen
        close(fd[1]);
        sm->thinker = getpid();
        performConnection(sockfd,gameId,playerNr);
    }

    close(sockfd);
    return 0;
}

//Spielverlauf, Feld auslesen, Gewinner ausgeben, Quit
int game(int socketfd, struct sharedMemory *sm) {
    char buffer[256] = {0};
    while (recv(socketfd,buffer,sizeof(buffer),0) != 0) {
        if (strstr(buffer, "+ ENDFIELD") != NULL) {
            send(socketfd,"THINKING\n",9*sizeof(char),0);
            bzero((char *) &buffer, sizeof(buffer));
            kill(sm->connector, SIGUSR1);
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



int readField(int socketfd, struct sharedMemory *sm) {
	char buffer[256] = {0};
    size_t size = sizeof(buffer);
	while(read(socketfd, buffer, size)) {
        if(strstr(buffer, "+ FIELD 8,8")) {
            bzero(buffer, size);
            for(int i = 0; i < 8; i++) {
                read(socketfd, buffer, size);
                
            }
        }
    }
    game(socketfd, sm);
    // anstoß des thinkers per SIGUSR1
    //kill(shm->thinker, SIGUSR1);
	return 0;
}



