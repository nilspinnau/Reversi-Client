CC = gcc
CFLAGS =  -Wall  -e 
TARGET = sysprak-client

all: $(TARGET)

#performConnection.o: performConnection.c performConnection.h
	#$(CC) $(CFLAGS) -c performConnection.c

$(TARGET): $(TARGET).c performConnection.c
	$(CC) $(CFLAGS)   performConnection.c $(TARGET).c -o $@

play: 
	./$(TARGET) -g $(GAME_ID) -p $(PLAYER) 

clean:
	rm -f $(TARGET).o $(TARGET)

