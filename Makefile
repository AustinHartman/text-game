CC = g++
CFLAGS  = -lncurses
RM = rm -f
PATH = src/
SRCS = $(wildcard src/*.o)


all: output

main.o: $(PATH)main.cpp
	$(CC) $(PATH)main.cpp -c

bullet.o: $(PATH)bullet.cpp $(PATH)bullet.h
	$(CC) $(PATH)bullet.cpp -c

enemy.o: $(PATH)enemy.cpp $(PATH)enemy.h
	$(CC) $(PATH)enemy.cpp -c

gameObject.o: $(PATH)gameObject.cpp $(PATH)gameObject.h
	$(CC) $(PATH)gameObject.cpp -c

player.o: $(PATH)player.cpp $(PATH)player.h
	$(CC) $(PATH)player.cpp -c

saber.o: $(PATH)saber.cpp $(PATH)saber.h
	$(CC) $(PATH)saber.cpp -c

output: $(PATH)main.o $(PATH)bullet.o $(PATH)enemy.o $(PATH)gameObject.o $(PATH)player.o $(PATH)saber.o
	$(CC) $^ -o output $(CFLAGS)

run: output
	./output

clean:
	$(RM) output $(SRCS)
