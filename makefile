<<<<<<< HEAD
CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = sysprak-client
GAME_ID = 1234567890123
PLAYER = 1


all: final


$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(TARGET).o

final: $(TARGET).o
	$(CC) $(CFLAGS) $(TARGET).o -o $(TARGET)
play: 
	./$(TARGET) data

clean:
	$(RM) $(TARGET).o $(TARGET)

=======
CC = gcc
CFLAGS =  -Wall -Wextra -Werror -e 
TARGET = sysprak-client

all: final

$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(TARGET).o
final: $(TARGET).o
	$(CC) $(CFLAGS) $(TARGET).o -o $(TARGET)
play: 
	./$(TARGET) -g $(GAME_ID) -p $(PLAYER) 

clean:
	rm -f $(TARGET).o $(TARGET)

>>>>>>> eb8a601040ea57a98c6f0548781d62e9eaa0ecf3
