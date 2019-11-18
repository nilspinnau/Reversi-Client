#include "performConnection.h"


int performConnection(int socketfd) {

    float version = 2.0;

    char data[256];

    while(recv(socketfd, data, sizeof(data), 0)) {
        switch(data[0]) {
            case '+':
                printf("S: %s", data);
                if(strstr(data, "+ MNM Gameserver")) {
                    char answer[50];
                    sprintf(answer, "ID %.2f", version);
                    send(socketfd, answer, strlen(answer), 0);
                } else if(strstr(data, "+ Client version accepted - please send Game-ID to join")) {

                } else if(strstr(data, "+ PLAYING")) {

                } else if(strstr(data, "+ Reversi")) {

                } else if(strstr(data, "+ YOU")) {

                } else if(strstr(data, "+ TOTAL")) {

                } else if(strstr(data, "+ ENDPLAYERS")) {

                } else {

                }
                break;       
            default:
                // disconnect
                perror(data);            
                close(socketfd);
                exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

int main(){

}


