CC = g++
CFLAGS = -Wall -Wextra

all:	server.o client.o

server.o:	headers/server.h src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp -o server.out

client.o:	headers/client.h src/client.cpp
	$(CC) $(CFLAGS) src/client.cpp -o client.out


clean:
	rm -f *.o *.out