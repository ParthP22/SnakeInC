CC = gcc
CFLAGS = -I./include -Wall -g
LDFLAGS = -L./lib/
LDLIBS = -lmingw32 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
OBJ = main.o game_state.o game_objects.o
HEADERS = main.h game_state.h game_objects.h

$(OBJ): main.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	rm -f *.o

clean:
	rm -f *.o
	rm -f *.exe



# OBJ = array_list.o reallocate.o single_linked_list.o
# HEADERS = array_list.h reallocate.h single_linked_list.h

# main: main.c $(OBJ) $(HEADERS)
# 	$(CC) $(CFLAGS) $(LDFLAGS) -o main $(OBJ) main.c  $(LDLIBS)

# make clean:
# 	rm -f *.o
# 	rm -f ./main.exe