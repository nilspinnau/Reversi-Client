#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include "performConnection.h"

#define HOSTNUMBER (1357)
#define GAMEKINDNAME ("Reversi")
#define HOSTNAME ("sysprak.priv.lab.nm.ifi.lmu.de")



int main(int argc, char **argv) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct hostent *hstnm;
    if((hstnm = gethostbyname(HOSTNAME)) == NULL) {
        perror("gethostbyname");
    }

    performConnection();

    exit(EXIT_SUCCESS);
}