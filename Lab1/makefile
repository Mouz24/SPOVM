CC = gcc

CFLAGS = -c -W -Wall -Wextra

.PHONY:all
all: start

start:	main.o	
	$(CC) Walkdir.o -o Walkdir

main.o: Walkdir.c
	$(CC) $(CFLAGS) main.c
	
clean:
	rm -rf *.o Walkdir