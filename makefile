CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= src/*.c
VAL = -ggdb3 -g
#LDFLAGS = -lm -lpthread
all: sysprak-client

/obj%.o: src/%.c lib/%.h
	$(CC) $(CFLAGS) -c $< -o $@
	
sysprak-client: $(OBJ)
	$(CC) $(CFLAGS) $(VAL) -o sysprak-client $(OBJ)

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

valgrind: sysprak-client
	valgrind --leak-check=full --trace-children=yes --show-leak-kinds=all --track-origins=yes ./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clean:
	rm -f *.o sysprak-client 
