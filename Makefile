CC = gcc
CFLAGS = -I./include -Wall -g
LDFLAGS = -L./lib/
LDLIBS = -lmingw32 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
OBJ = main.o game_state.o game_objects.o
HEADERS = include/main.h include/game/game_state.h include/game/game_objects.h
TARGET = snake-in-c-v1.0.0.exe

%.o: ./src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	rm -f $(OBJ)

make clean:
	rm -f %.exe


