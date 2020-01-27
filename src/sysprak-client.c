#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#include "../lib/signalHandler.h"
#include "../lib/performConnection.h"
#include "../lib/getconfig.h"
#include "../lib/handler.h"
#include "../lib/think.h"
#endif


#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"



sharedMemory *sm;
int fd[2];

int main(int argc, char **argv) {
    int opt;
    char gameId[14] = {0};
    int playerNr;
    char path[256] = {0};
    strcpy(path,"config.conf");
    configs res;
    configs* rp;
    memset(&res,0,sizeof(configs));

    // flags
    bool gFlag = false;
    bool pFlag = false;

    while ((opt = getopt (argc, argv, "g:p:f:")) != -1) {
        switch (opt) {
            case 'g':
                gFlag = true;
                if (strlen(optarg) != 13) {
					printf("Bitte 13-stellige Game-Id eingeben\n");
					exit(EXIT_FAILURE);
                }
                else {
                    strcpy(gameId, optarg);
                } 
                break;
            case 'p':
                pFlag = true;
                playerNr = atoi(optarg);
                if (playerNr < 1 || playerNr > 2) {
                	printf("Spieler 1 oder 2\n");
					exit(EXIT_FAILURE);
				}
                break;
            case 'f':
                bzero((char*) &path, sizeof(path));
                strcpy(path,optarg);
                break;    
        }
    }

    if(!gFlag) {
        printf("Keine GameId\n");
        exit(EXIT_FAILURE);
    }


    rp = getconfig(&res,path);
    if(rp == NULL){
        printf("configfile err");
        return 0;
    }
   
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
    if(pipe(fd)<0) {
        perror("Fehler beim Einrichten der Pipe.\n");
        exit(EXIT_FAILURE);
    }

    // check if playerNr is initialized, if not change value to 3 for error handling
    if(!pFlag) {
        sm->me.playerNr = 3;
        playerNr = 3;
    }

    if((pid = fork()) < 0) {
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
        while((pid=waitpid(sm->connector,NULL,WNOHANG)) == 0);
        /* while((pid=waitpid(sm->connector,NULL,WNOHANG)) == 0){
            if(sm->thinkFlag){
                write(fd[1],think(sm),3*sizeof(char));
                sm->thinkFlag = false;
            }
        } */
    }
    
    /*
    * Connector = Kindprozess
    */
    else {
        // Schreibseite der Pipe schliessen
        close(fd[1]);
        performConnection(sockfd,gameId,playerNr,fd); 
    }

    shmdt((void*)sm);
    shmctl(shm_id, IPC_RMID, NULL);
    close(sockfd);
 
    return 0;
}