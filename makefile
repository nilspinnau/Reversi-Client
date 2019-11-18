CC:= gcc
CFLAGS := -Wall -Wextra -Werror

TARGET:= sysprak-client
VAR:= GAME_ID<GAME-ID> PLAYER=<{1,2}>
all: play

$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(TARGET).o

play: $(TARGET).o
	echo $(VAR)
	$(CC) $(CFLAGS) $(TARGET).o $(TARGET)

clean:
	$(RM) $(TARGET).o $(TARGET)

