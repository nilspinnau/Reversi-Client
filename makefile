CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= *.c
#LDFLAGS = -lm -lpthread
all: sysprak-client

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
sysprak-client: $(OBJ)
	$(CC) $(CFLAGS)  -o sysprak-client $(OBJ)

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f *.o sysprak-client 
