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
valgrind:	
	valgrind --leak-check=full --trace-children=yes --show-leak-kinds=all --track-origins=yes -v ./sysprak-client -g $(GAME_ID) -p $(PLAYER) 
clean:
	rm -f *.o sysprak-client 
