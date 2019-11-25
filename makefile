CC = /usr/bin/gcc
<<<<<<< HEAD
CFLAGS =  -Wall
=======
CFLAGS = -Werror -Wextra -Wall
>>>>>>> d55f394c5bc138b635158c6d2d88e057545a4b53
LDFLAGS = -lm -lpthread

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
>>>>>>> d55f394c5bc138b635158c6d2d88e057545a4b53

clear:
	rm -f sysprak-client.o sysprak-client performConnection.o

