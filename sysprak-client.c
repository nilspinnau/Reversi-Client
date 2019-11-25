#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "performConnection.h"

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char **argv) {

    int opt;
    int gameId;
    int playerNr;

     while ((opt = getopt (argc, argv, "g:p:")) != -1) {
        switch (opt) {
            case 'g':
 	            gameId = atoi(optarg);
	            break;
            case 'p':
                playerNr = atoi(optarg);
	            break;
        }
    }
    int sockfd, portno; //,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //char buffer[256];
    portno = PORTNUMBER;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");
    bzero((char* ) &serv_addr, sizeof(serv_addr));
    

    
    server = gethostbyname(HOSTNAME);
    if (server == NULL) {
        perror("ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server-> h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
    }
    /*bzero(buffer,256);
    fgets(buffer,255,stdin); 
    
    n = read(sockfd,buffer,255);
    if (n < 0)
        perror("ERROR reading from socket");
    printf("%s\n",buffer);
    */
    
    /*n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        perror("ERROR writing to socket");
    bzero(buffer,256);*/
    int fd[2];
    pid_t pid =0;
    int ret_code =0;
    fd[0]=fd[1]=0;
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
    ret_code = waitpid(pid, NULL, 0);
  if (ret_code < 0) {
      perror ("Fehler beim Warten auf Kindprozess.");
      exit(EXIT_FAILURE);
    }  
}
  /*
   * Connector = Kindprozess
   */
  else {
      // Schreibseite der Pipe schliessen
    close(fd[1]);
   
   
    performConnection(sockfd);
  }
  
    return 0;
    }
