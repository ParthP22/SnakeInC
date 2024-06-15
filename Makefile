CC = gcc
CFLAGS = -I./include -Wall -g
LDFLAGS = -L./lib/
LDLIBS = -lmingw32 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf


%.o: main.c main.h
	$(CC) $(CFLAGS) -c -o $@ $<

main: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)




clean:
	rm -f main.o
	rm -f main.exe
	rm -f a.exe