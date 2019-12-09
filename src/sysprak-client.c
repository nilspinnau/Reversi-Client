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
#include <time.h>

#include "../lib/signalHandler.h"
#include "../lib/performConnection.h"
#include "../lib/think.h"
#include "../lib/getconfig.h"
#include "../lib/handler.h"

struct player {
    int playerNr;
    char playerName[10];
    bool registered;
};

struct sharedMemory {
    struct player p;
    char gameName[10];
    int playerNr;
    int playerCount;
    pid_t thinker;
    pid_t connector;
};


/*
#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
*/

char *buff;
char field[8][8];

int cleanExit(char *s) {
    free(buff);
    printf("%s\n", s);
    return EXIT_FAILURE;
}

int readField() {
	if(isnext("+ FIELD 8,8")) {
        for(int i=0;i<8;i++) {
            char* line = getLine();
            for(int j=0;j<8;j++) {
                field[i][j] = line[j+1];
            }
        }
        handler(SIGUSR1,field);

    } else {
        return cleanExit("isnext ist leer");
    }
    return 0;
}

//Spielverlauf, Feld auslesen, Gewinner ausgeben, Quit
int game() {
    if(isnext("+ MOVE")) {
        readField();
    } else if(isnext("+ WAIT")) {
        toServer("OKWAIT\n");
    } else if(isnext("+ GAMEOVER")) {
        char *buff;
        while((buff = getLine()) != NULL) {
            printf("S: %s", buff);
        }
    }
    return 0;
}

int main(int argc, char **argv) {

    buff = malloc(256*8);
    // alles drüber free()

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
    setSocket(sockfd, buff);
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

    printf("test\n");
    int fd[2];
    pid_t pid =0;
    int ret_code =0;
    fd[0]=fd[1]=0;
    //struct sharedMemory* sm = malloc(sizeof(struct sharedMemory));
    //int shm_id = shmget(IPC_PRIVATE,sizeof(struct sharedMemory),0);
    //sm = (struct sharedMemory*) shmat(shm_id,NULL,0);

    
    printf("test\n");

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
        // ab hier unklar
        ret_code = waitpid(pid, NULL, 0);
        signal(SIGUSR1, handler);
        if (ret_code < 0) {
            perror ("Fehler beim Warten auf Kindprozess.");
            exit(EXIT_FAILURE);
        }  
        char *answer = think(field);
        write(fd[1], answer, sizeof(answer));
    }
    
    /*
    * Connector = Kindprozess
    */
    else {
        // Schreibseite der Pipe schliessen
        close(fd[1]);
        performConnection(sockfd,gameId,playerNr);
        readField(sockfd);
    }

    close(sockfd);
    return 0;
}




