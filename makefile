CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= *.c
#LDFLAGS = -lm -lpthread
all: sysprak-client

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

sysprak-client: $(OBJ)
	$(CC) $(CFLAGS) -o sysprak-client $(OBJ)

#performConnection.o: performConnection.h performConnection.c
#	$(CC) $(CFLAGS) -c performConnection.c

#sysprak-client: performConnection.o sysprak-client.c 
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o sysprak-client.c

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f sysprak-client.o sysprak-client performConnection.o
