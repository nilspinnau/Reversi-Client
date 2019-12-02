CC = /usr/bin/gcc
CFLAGS = -Werror -Wextra -Wall
OBJ= *.c
#LDFLAGS = -lm -lpthread
all: sysprak-client

<<<<<<< HEAD
%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

sysprak-client: $(OBJ)
	$(CC) $(CFLAGS) -o sysprak-client $(OBJ)

#performConnection.o: performConnection.h performConnection.c
#	$(CC) $(CFLAGS) -c performConnection.c

#sysprak-client: performConnection.o sysprak-client.c 
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o sysprak-client.c
=======
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
sysprak-client: $(OBJ)
	$(CC) $(CFLAGS)  -o sysprak-client $(OBJ)
>>>>>>> 8623844357c5ae9e2f9c1eb7f809f77535204f06

play: 
	./sysprak-client -g $(GAME_ID) -p $(PLAYER) 

clear:
	rm -f *.o sysprak-client 
