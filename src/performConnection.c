#include "../lib/performConnection.h"


int performConnection(int socketfd, char *gameId, int playerNr) {

    char *version = "2.3";
    char buffer[256] = {0};
    size_t size = sizeof(buffer);
/* 
    bool equals = false;
    printf("%d\n", equals);

    bool test = true;
    printf("%d\n", test);2g330snu9j3z0

    while(true) {
        if(isNext(socketfd, "+ MNM Gameserver")) {
            printf("S: MNM Gameserver\n");
            write(socketfd, "VERSION 2.3\n", 11*8);
        }
        if(isNext(socketfd, "+ Client version accepted")) {
            printf("S: Client accepted - send Game-ID\n");
            write(socketfd, "ID\n", 17*8);
            bzero(buffer, sizeof(buffer));
            read(socketfd, buffer, sizeof(buffer));
            printf("%s", buffer);
            break;
        }
        if(isNext(socketfd, "- ")) {
            printf("- Fehlermeldung");
            exit(EXIT_FAILURE);
        }
    } */
/* 
    isnext(socketfd, &equals, "+ MNM Gameserver");
    if(equals) {
        twoServer(socketfd, "VERSION ", version);
        //write(socketfd, "VERSION 2.3\n", 12*8);
        equals = false;
    }
    isnext(socketfd, &equals, "+ Client version accpeted");
    if(equals) {
        //twoServer(socketfd, "ID ", gameId);
        write(socketfd, "ID 2r8va45yerq3z\n", 16*8);
        equals = false;
    }
    bzero(buffer, sizeof(buffer));
    read(socketfd, buffer, sizeof(buffer));
    printf("%s", buffer);
    getLine(socketfd, buffer); */
/* 
    getLine(socketfd, buffer);
    write(socketfd, "VERSION 2.3\n", 12*sizeof(char));
    getLine(socketfd, buffer);
    write(socketfd, "ID 3hhwkh9xomkna\n", 17*sizeof(char));
    //twoServer(socketfd, "ID ", gameId);
    getLine(socketfd, buffer);
    //line(socketfd);
    write(socketfd, "PLAYER\n", 7*8);
    line(socketfd);
    //read(socketfd, buffer, sizeof(buffer));
    //read(socketfd, buffer, sizeof(buffer));

 */

    while(read(socketfd, buffer, size) != 0) {
       printf("S: %.*s", (int) strlen(buffer), buffer);

            if(strstr(buffer, "+ MNM Gameserver")) {
                // sending client id/version
                bzero(buffer, size);
                sprintf(buffer, "VERSION %s%s", version, "\n\0");
                write(socketfd, buffer, strlen(buffer));
                printf("C: %.*s", (int) strlen(buffer), buffer);
            } else if(strstr(buffer, "+ Client version accepted - please send Game-ID to join")) {
                // sending GameId
                bzero(buffer, sizeof(buffer));
                sprintf(buffer, "ID %s%s", gameId, "\n");
                //bzero(buffer, sizeof(buffer));
                write(socketfd, buffer, strlen(buffer));
                printf("C: %.*s", (int) strlen(buffer), buffer);
            } else if(strstr(buffer, "+ PLAYING")) {
                // sending PlayerNr
                bzero(buffer, size);
                read(socketfd, buffer, size);
                printf("S: %.*s", (int) strlen(buffer), buffer);
                bzero(buffer, size);
                write(socketfd,"PLAYER\n",7*8);
                printf("C: PLAYER %d \n",playerNr);
            } else if(strstr(buffer, "+ ENDPLAYERS")) {
                // go on to readField and game in sysprak-client 
                exit(EXIT_SUCCESS);
            } else {
                // disconnect from server, error message or wrong message
                close(socketfd);
                exit(EXIT_FAILURE);
            }
            bzero(buffer, size);
    }
    exit(EXIT_SUCCESS);
}
