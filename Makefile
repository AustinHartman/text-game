CC = g++
CFLAGS  = -lncurses

default: output

output: main.o player.o bullet.o enemy.o gameObject.o saber.o
	$(CC) -o output main.o player.o bullet.o enemy.o gameObject.o saber.o -lncurses

main.o: main.cpp
	$(CC) -c main.cpp

player.o: player.cpp player.h
	$(CC) -c player.cpp

bullet.o: bullet.cpp bullet.h
	$(CC) -c bullet.cpp

enemy.o: enemy.cpp enemy.h
	$(CC) -c enemy.cpp

saber.o: saber.cpp saber.h
	$(CC) -c saber.cpp

gameObject.o: gameObject.cpp gameObject.h
	$(CC) -c gameObject.cpp

run: output
	./output

clean:
	$(RM) output *.o
