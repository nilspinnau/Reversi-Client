CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
<<<<<<< HEAD
=======
LDFLAGS = -lm -lpthread
>>>>>>> 523c5049ab7aa334747731af9b8ec8384589548d

all: sysprak-client

performConnection.o: performConnection.h performConnection.c
	$(CC) $(CFLAGS) -c performConnection.c
<<<<<<< HEAD

sysprak-client: performConnection.o sysprak-client.c 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o sysprak-client.c

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 
=======

sysprak-client: performConnection.o sysprak-client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o sysprak-client.c

play:
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)

clear:
	rm -f sysprak-client.o sysprak-client performConnection.o
>>>>>>> 523c5049ab7aa334747731af9b8ec8384589548d

clear:
	rm -f sysprak-client.o sysprak-client performConnection.o
