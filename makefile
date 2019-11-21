CC = /usr/bin/gcc
WFLAGS = -Werror -Wextra -Wall

all: sysprak-client

performConnection.o: performConnection.h performConnection.c
	$(CC) $(WFLAGS) -c performConnection.c

sysprak-client: sysprak-client.c performConnection.o
	$(CC) $(WFLAGS) -o $@ performConnection.o sysprak-client.c

clear:
	rm -f sysprak-client.o sysprak-client

