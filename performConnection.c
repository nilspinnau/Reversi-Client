#include "performConnection.h"


// vielleicht noch fehlerbehandlung von send bearbeiten
int performConnection(int socketfd) {

    float version = 2.0;
    int GAME_ID = 0;

    char data[256];

    while(recv(socketfd, data, sizeof(data), 0) != 0) {
        switch(data[0]) {
            case '+':
                printf("S: %s", data);
                if(strstr(data, "+ MNM Gameserver")) {
                    // sending client id/version
                    char answer[50];
                    sprintf(answer, "ID %.2f", version);
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ Client version accepted - please send Game-ID to join")) {
                    // sending gameID
                    char answer[50];
                    sprintf(answer, "%d", GAME_ID);
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ PLAYING Reversi")) {

                } else if(strstr(data, "+ YOU")) {

                } else if(strstr(data, "+ TOTAL")) {

                } else if(strstr(data, "+ ENDPLAYERS")) {

                } else if(strstr(data, "+ WAIT")) {
                    char answer[6] = "OKWAIT";
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ MOVE")) {

                } else if(strstr(data, "+ FIELD")) {

                } else if(strstr(data, "+ ENDFIELD")) {
                    // sending thinking answer
                    char answer[8] = "THINKING";
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ QUIT")) {
                    // game ends -> servers sends quit signal
                    perror(data);
                    close(socketfd);
                    exit(EXIT_SUCCESS);
                } 

                break;       
            default:
                // disconnect
                printf("S: %s", data);            
                close(socketfd);
                exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

int main(){

}


