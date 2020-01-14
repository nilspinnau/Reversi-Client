#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#include "../lib/signalHandler.h"
#include "../lib/performConnection.h"
#include "../lib/getconfig.h"
#include "../lib/handler.h"
#include "../lib/gameloop.h"
#include "../lib/readField.h"
#include "../lib/think.h"
#endif


/*
#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
*/



/*
char playerName[20];// durch shared memory sm-> playerName ersetzt!
int myplayerNr;// durch shared memory sm-> myplayerNr ersetzt!
char feld[8][8];
*/
//readfield error handle?

sharedMemory *sm;

int main(int argc, char **argv) {
    init();
    int opt;
    char gameId[14];
    int playerNr;
    char path[256];
    strcpy(path,"config.conf\n");
    configs res;
    configs* rp;
    memset(&res,0,sizeof(configs));

    while ((opt = getopt (argc, argv, "g:p:f:")) != -1) {
        switch (opt) {
            case 'g':
                if (strlen(optarg) != 13) {
					printf("Bitte 13-stellige Game-Id eingeben\n");
					exit(EXIT_FAILURE);
                }
                else {
                strcpy(gameId, optarg);
                } 
                break;
            case 'p':
                playerNr = atoi(optarg);
                if (playerNr < 0 || playerNr > 1) {
                	printf("Spieler 1 oder 0\n");
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
        perror("ERROR connecting\n");
    }

    pid_t pid =0;
    int fd[2];
    //int ret_code =0;
    fd[0]=fd[1]=0;
    
    int shm_id ;
    // get ID of Shared Memory Segment   
    shm_id = shmget(IPC_PRIVATE,sizeof(sharedMemory),IPC_CREAT | 0666 );
        if(shm_id < 0) {
            printf("shmget ERROR\n");
            exit(EXIT_FAILURE);
        }
    // attach the Shared Memory Segment
    sm = (sharedMemory*) shmat(shm_id,NULL,0);
        if((sharedMemory*) sm  ==  (sharedMemory*) -1) {         
            printf("shmat Error\n");
            exit(EXIT_FAILURE);
         }
    sm->thinkFlag=false;
    //Pipe erzeugen
        if(pipe(fd)<0){
            perror("Fehler beim Einrichten der Pipe.\n");
            exit(EXIT_FAILURE);
        }
    pid = fork();
    
    if (pid < 0) {
        perror ("Fehler bei fork().\n");
        exit(EXIT_FAILURE);
    }
    /*
    * THINKER = ELTERNPROZESS
    */
    if(pid >0){
        // READSEITE der Pipe schliessen
        close(fd[0]);
        sm->connector=pid;
        signal(SIGUSR1, signalhandler);
        //ret_code = waitpid(pid, NULL, 0);
        while((pid=waitpid(sm->connector,NULL,WNOHANG)) == 0){
            if(sm->thinkFlag){
            write(fd[1],think(sm),3*sizeof(char));
            sm->thinkFlag = false;
            }
        }
        /*
        if (ret_code < 0) {
            perror ("Fehler beim Warten auf Kindprozess.");
            exit(EXIT_FAILURE);
        } 
        */ 
        //char *answer = think(sm);
        //write(fd[1], answer, sizeof(answer));
    }
    
    /*
    * Connector = Kindprozess
    */
    else {
        // Schreibseite der Pipe schliessen
        close(fd[1]);
        if(!performConnection(sockfd,gameId,playerNr,fd)){
            printf("Failed Server connection\n");
            return EXIT_FAILURE;
        }
        if(gameloop(sm,fd)){
            printf("Game finito\n");
            return EXIT_SUCCESS;
        }
        else{
            printf("Game finito Fehler\n");
            return EXIT_FAILURE;
        }
    }
    shmdt((void*)sm);
    shmctl(shm_id, IPC_RMID, NULL);
    close(sockfd);
    return 0;
}