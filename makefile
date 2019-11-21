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

