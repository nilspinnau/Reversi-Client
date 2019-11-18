#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAMEKINDNAME "Reversi"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char **argv) {
    long i = strtol(argv[1], NULL, 10);
    if (argc != 2) {
        printf("Bitte geben Sie eine 13-stellige Game-ID und die gewünschte Spielernummer an");
        return -1;
    }
    if (i < 1 || i > 2) {
        printf("Spielerzahl muss entweder 1 oder 2 sein");
        return -1;
    }
    if (strlen(argv[0]) != 13) {
        printf("Game-ID muss 13-stellig sein");
        return -1;
    }
    return 0;
}