CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
LDFLAGS = -lm -lpthread
all: sysprak-client

performConnection.o: performConnection.h performConnection.c
	$(CC) $(CFLAGS) -c performConnection.c

sysprak-client: performConnection.o sysprak-client.c 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o sysprak-client.c

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f sysprak-client.o sysprak-client performConnection.o
