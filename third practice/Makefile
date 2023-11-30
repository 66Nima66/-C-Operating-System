CC=gcc

CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = main
OBJS = main.o mem.o

all: $(TARGET)

main: main.o mem.o
	$(CC) mem.o main.o -o main -lpthread -lcurl

mem.o: mem.c
	$(CC) $(CFLAGS) -c mem.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	/bin/rm -f main *.o *.gz

run:
	./main

tarball:
	tar -cvzf LASTNAME.tar.gz *
