CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= src/*.c
# for valgrind, shows line in which error occurs 
VAL = -ggdb3
#LDFLAGS = -lm -lpthread
all: sysprak-client

/obj%.o: src/%.c lib/%.h
	$(CC) $(CFLAGS) -c $< -o $@
	
sysprak-client: $(OBJ)
	$(CC) $(CFLAGS) $(VAL) -o sysprak-client $(OBJ)

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f *.o sysprak-client 
