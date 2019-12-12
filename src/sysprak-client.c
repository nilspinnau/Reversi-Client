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

extern char buff[256];

struct player {
    int playerNr;
    char playerName[10];
    bool registered;
};
struct spielFeld{
    int width;
    int height; 
    char Feld[8][8];
};

struct sharedMemory {
    struct player p;
    struct spielFeld field; 
    char gameName[10];
    int playerNr;
    int playerCount;
    pid_t thinker;
    pid_t connector;
};

struct spielFeld field;
/*
#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
*/

char *loopbuffer;


//readfield error handle?
 
int readField(char *buffer) {
    char* line ;
    printf("hello: %s", buffer);
    //loopbuffer = getLine();
    while ( (line=strtok(buffer,"\n")) != NULL){
       buffer = NULL; // strtok wird beim nächste Mal mit NULL aufgerufen
       int row = line[2] -'1';
       sscanf(line, "+ %*i %c %c %c %c %c %c %c %c", 
       &field.Feld[row][0],&field.Feld[row][1],&field.Feld[row][2],
       &field.Feld[row][3],&field.Feld[row][4],&field.Feld[row][5],
       &field.Feld[row][6],&field.Feld[row][7]);
    }
    
    for(int i=0; i<8; i++){
        for(int j=0; j<8;j++){
       printf("%c", field.Feld[i][j]);
    }
        printf("\n");
       }
     return 0;
}

void gameloop(){
    bool exit = false;
    //only when player 2 for testing test:
    //toServer("THINKING\n");
    while(!exit){
        loopbuffer = getLine();
        if(strcmp(loopbuffer,"+ WAIT\n") == 0){
            toServer("OKWAIT\n");
        }
        if(strcmp(loopbuffer,"GAMEOVER\n") == 0){
            readField(buff);
            //handle who is winner
            break;
        }
        if(strcmp(loopbuffer,"- TIMEOUT Be faster next time\n") == 0){
            printf("Timeout -> Exiting Programm");
            break;
        }
        if(strncmp(loopbuffer,"+ MOVE ",7) == 0){
            readField(buff);
            //thinker anstoßen
        }        
    }

}
/*
//Spielverlauf, Feld auslesen, Gewinner ausgeben, Quit
int game(int sf, pid_t pid) {
    if(isNext(sf, "+ MOVE")) {
        readField(sf);
    } else if(isNext(sf, "+ WAIT")) {
        toServer("OKWAIT\n");
    } else if(isNext(sf, "+ GAMEOVER")) {
        char *buff = malloc(256*8);
        getLine(sf, buff);
        while(buff != NULL) {
            printf("S: %s", buff);
            getLine(sf, buff);
        }
        free(buff);
    } else if(strstr(buff, "ENDFIELD")) {
        write(sf, "THINKING\n", 9*8);
        kill(pid, SIGUSR1);
    }
    return 0;
}
*/

int main(int argc, char **argv) {

    init();
    //buff = malloc(256*8);
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
            case 'p':
                playerNr = atoi(optarg);
                if (playerNr < 1 || playerNr > 2) {
                	perror("Spieleranzahl 1 oder 2");
					exit(EXIT_FAILURE);
				}
                break;
            case 'f':
                //bzero((char*) &path, sizeof(path));
                strcpy(path,optarg);
                 
                break;    
        }
    }
    rp = getconfig(&res,path);
    if(rp == NULL){
        printf("configfile err");
        return 0;
    }/* 
    printf("%s\n",rp->game_kind);
    printf("%s\n",rp->host_name);
    printf("%d\n",rp->port_nr); */
   
    int sockfd;
    //portno
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setSocket(sockfd);
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
    struct sharedMemory* sm = malloc(sizeof(struct sharedMemory));
    int shm_id = shmget(IPC_PRIVATE,sizeof(struct sharedMemory),0);
    sm = (struct sharedMemory*) shmat(shm_id,NULL,0);
    printf ("shared memory attached at address %p\n", sm);
    
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
        sm->thinker = getppid();
        // ab hier unklar
        ret_code = waitpid(pid, NULL, 0);
        // signal(SIGUSR1, handler);
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
        if(!performConnection(sockfd,gameId,playerNr))return EXIT_FAILURE;
        readField(buff);
        printf("Hi\n");
        gameloop();
        //readField();
         printf("Hi\n");
    }

    close(sockfd);
    return 0;
}   



