output: main.o player.o bullet.o enemy.o gameObject.o saber.o
	g++ -o output main.o player.o bullet.o enemy.o gameObject.o saber.o -lncurses

main.o: main.cpp
	g++ -c main.cpp

player.o: player.cpp player.h
	g++ -c player.cpp

bullet.o: bullet.cpp bullet.h
	g++ -c bullet.cpp

enemy.o: enemy.cpp enemy.h
	g++ -c enemy.cpp

saber.o: saber.cpp saber.h
	g++ -c saber.cpp

gameObject.o: gameObject.cpp gameObject.h
	g++ -c gameObject.cpp

run: output
	./output
