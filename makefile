CC = g++
CFLAGS = -std=c++11
LDFLAGS = -lncurses

all: snakegame

snakegame: item.o map.o position.o snake.o stage.o
	$(CC) $(CFLAGS) item.o map.o position.o snake.o stage.o -o snakegame $(LDFLAGS)

item.o: item.cpp
	$(CC) $(CFLAGS) -c item.cpp

map.o: map.cpp
	$(CC) $(CFLAGS) -c map.cpp

position.o: position.cpp
	$(CC) $(CFLAGS) -c position.cpp

snake.o: snake.cpp
	$(CC) $(CFLAGS) -c snake.cpp

stage.o: stage.cpp
	$(CC) $(CFLAGS) -c stage.cpp

clean:
	rm -f *.o snakegame

