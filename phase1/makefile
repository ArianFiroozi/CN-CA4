CC = g++
CFLAGS = -Wall -Wextra

all: server.o client.o

# main.o: 
# 	$(CC) $(CFLAGS) src/server.cpp src/client.cpp main.cpp -o main.out

server.o:	headers/server.h src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp -o server.o

client.o:	headers/client.h src/client.cpp
	$(CC) $(CFLAGS) src/client.cpp -o client.o


clean:
	rm -f *.o *.out