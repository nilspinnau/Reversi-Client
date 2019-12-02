CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= src/*.c
#LDFLAGS = -lm -lpthread
all: sysprak-client

/obj%.o: src/%.c lib/%.h
	$(CC) $(CFLAGS) -c $< -o $@
	
sysprak-client: $(OBJ)
	$(CC) $(CFLAGS)  -o sysprak-client $(OBJ)

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f *.o sysprak-client 
