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
#include "../lib/getconfig.h"
#include "../lib/struct_H.h"
#include "../lib/handler.h"

//Hilfsfunktion um Leerzeichen aus der Servernachricht zu loeschen
void remove_spaces(char *str) { 
    int count = 0; 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ') 
            str[count++] = str[i]; 
    str[count] = '\0'; 
} 

/*
#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
*/

char *loopbuffer;
char playerName[20];// durch shared memory sm-> playerName ersetzt!
int myplayerNr;// durch shared memory sm-> myplayerNr ersetzt!
char feld[8][8];

//readfield error handle?
bool readField() {
    if(strcmp(loopbuffer,"+ FIELD 8,8")!=0)return false;
    
    for(int row = 0;row < 8;row++){
        loopbuffer = nextbufLine();
        sscanf(loopbuffer,"+ %*c %c %c %c %c %c %c %c %c",&feld[row][0],&feld[row][1],&feld[row][2],&feld[row][3],&feld[row][4],&feld[row][5],&feld[row][6],&feld[row][7]);
    }
    loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ ENDFIELD")!=0)return false;
    return true;
}

bool setPlayer(){
    loopbuffer = getbuffer();
    if(sscanf(loopbuffer,"+ YOU %d %*s player\n",&myplayerNr)!= 1)return false;// durch shared memory sm-> myplayerNr ersetzt!
    getLine();// Line Total 2 .. Endplayer .. Field/wait
    loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ TOTAL 2")!=0)return false;
       loopbuffer = nextbufLine();
    if(sscanf(loopbuffer,"+ %*d %s player %*d",playerName)!= 1)return false;// durch shared memory sm-> playerName ersetzt!
       loopbuffer = nextbufLine();
    if(strcmp(loopbuffer,"+ ENDPLAYERS")!=0)return false;
    if(myplayerNr == 1){
        loopbuffer = getLine();
    }
    else{
        loopbuffer = nextbufLine();
    }

    
return true;
}
void gameloop(){
    bool exit = false;
    //printf("%s", loopbuffer);

    while(!exit){
        if(strcmp(loopbuffer,"+ WAIT\n") == 0){
            toServer("OKWAIT\n");
        }
        if(strcmp(loopbuffer,"GAMEOVER\n") == 0){
            readField();
            //handle who is winner
            break;
        }
        if(strcmp(loopbuffer,"- TIMEOUT Be faster next time\n") == 0){
            printf("Timeout -> Exiting Programm");
            break;
        }
        if(strcmp(loopbuffer,"+ MOVE 3000") == 0){ //nur beim 1. mal -Teil von größerem String
            loopbuffer = nextbufLine();
            if(!readField()){
                printf("Field could not be read");
                break;
            }
            toServer("THINKING\n");
            /*to test (working)
            for(int i = 0;i < 8;i++){
                for(int j= 0;j < 8;j++){
                    printf("%c",spielFeld[i][j]);
                }
                printf("\n");
            }*/
            getLine();
            //break;//entfernen wenn thinker funtioniert
            //thinker anstoßen
            toServer("PLAY D6\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move");
                break;
            }
        

        }

        if(strcmp(loopbuffer,"+ MOVE 3000\n") == 0){
            getLine();
            resetLinebuf();
            loopbuffer = nextbufLine();
            printf("%s",loopbuffer);
            if(!readField()){
                printf("Field could not be read");
                break;
            }
            toServer("THINKING\n");
            /*to test (working)
            for(int i = 0;i < 8;i++){
                for(int j= 0;j < 8;j++){
                    printf("%c",spielFeld[i][j]);
                }
                printf("\n");
            }*/
            getLine();
            //break;//entfernen wenn thinker funtioniert
            //thinker anstoßen
            toServer("PLAY D6\n");
            if(!isnext("+ MOVEOK\n")){
                printf("Invalid Thinker move");
                break;
            }
        

        }    
        loopbuffer = getLine();        
    }

}

int main(int argc, char **argv) {

    init();
    
    int opt;
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
					printf("Bitte 13-stellige Game-Id eingeben");
					exit(EXIT_FAILURE);
                }
                else {
                strcpy(gameId, optarg);
                } 
                break;
            case 'p':
                playerNr = atoi(optarg);
                if (playerNr < 0 || playerNr > 1) {
                	printf("Spieler 1 oder 0");
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
    struct spielFeld* field = malloc(sizeof(struct spielFeld));
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
        field->Feld = feld;
        sm->feld = field;
        // ab hier unklar
        ret_code = waitpid(pid, NULL, 0);
        // signal(SIGUSR1, handler);
        if (ret_code < 0) {
            perror ("Fehler beim Warten auf Kindprozess.");
            exit(EXIT_FAILURE);
        }  
        char *answer = think(sm);
        write(fd[1], answer, sizeof(answer));
    }
    
    /*
    * Connector = Kindprozess
    */
    else {
        // Schreibseite der Pipe schliessen
        close(fd[1]);
        if(!performConnection(sockfd,gameId,playerNr)){
            printf("failed Server connection");
            return EXIT_FAILURE;
        }
        if(!setPlayer()){
            printf("Player can't be set");
            return EXIT_FAILURE;
        }
        gameloop();
    }

    close(sockfd);
    return 0;
}