CC = g++
CFLAGS  = -lncurses
RM = rm -f

all:
	$(CC) src/*.cpp -o output -lncurses

run: all
	./output

clean:
	$(RM) output
