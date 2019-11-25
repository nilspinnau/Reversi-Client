#include "performConnection.h"

#define BUF 256
// vielleicht noch fehlerbehandlung von send bearbeiten
int performConnection(int sockfd) {

    /*float version = 2.0;
    char GAME_ID[13];

    char data[256];
    */
    char *buffer = (char*) malloc(sizeof(char)*BUF);
    ssize_t size;
   
    do {
    // Der Client wartet auf die erste Nachricht des Servers
    size = recv(sockfd, buffer, BUF-1, 0);
    if(buffer[0] == '+' ){
      buffer[size]='\0';
      printf("S: %s", buffer);
   
      printf("C: VERSION ");
      fgets(buffer, BUF, stdin);
      send(sockfd,buffer,strlen(buffer),0);
    }else{
       EXIT_FAILURE;
    }
    
      }while (strcmp (buffer, "quit\n")!=0);
    close(sockfd);    
    free(buffer);
    exit(EXIT_SUCCESS);
}

    /*while(recv(socketfd, data, sizeof(data), 0) > 0) {
        switch(data[0]) {
            case '+':
                printf("S: %s", data);

                // part of Spielverlauf but implemented earlier to get a faster answer to Server
                if(strstr(data, "+ WAIT")) {
                    // sending okwait answer
                    char answer[6] = "OKWAIT";
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ ENDFIELD")) {
                    // sending thinking answer
                    char answer[8] = "THINKING";
                    send(socketfd, answer, strlen(answer), 0);
                    // signal send to Thinker process to figure out a move

                // begining of Prolog
                } else if(strstr(data, "+ MNM Gameserver")) {
                    // sending client id/version
                    char answer[3];
                    sprintf(answer, "ID %.2f", version);
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ Client version accepted - please send Game-ID to join")) {
                    // sending gameID, is it an int or char???
                    char answer[13];
                    sprintf(answer, "%s", GAME_ID);
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ PLAYING")) {
                    // nothing happens, server sends another message
                } else if(strstr(data, "+ GAMENAME")) {
                    char answer[6] = "PLAYER";
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ YOU")) {
                    // writes back Mitspielernummer and Mitspielername
                } else if(strstr(data, "+ TOTAL")) {
                    // writes back Mitspieleranzahl
                } else if(strstr(data, "+ ENDPLAYERS")) {




                   // end of Prolog, Spielverlauf begins
                } else if(strstr(data, "+ MOVE")) {

                } else if(strstr(data, "+ FIELD")) {

                } else if(strstr(data, "+ QUIT")) {
                    // game ends -> servers sends quit signal
                    perror(data);
                    close(socketfd);
                    exit(EXIT_SUCCESS);
                } 

                break;       
            default:
                // disconnect, error message from server
                printf("S: %s", data);            
                close(socketfd);
                exit(EXIT_FAILURE);
        }
    } 
    exit(EXIT_SUCCESS);
}
*/


