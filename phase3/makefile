CC = g++
CFLAGS = -Wall -Wextra

all: receiver.o sender.o router.o

# main.o: 
# 	$(CC) $(CFLAGS) src/receiver.cpp src/sender.cpp main.cpp -o main.out

receiver.o:	headers/receiver.h src/receiver.cpp
	$(CC) $(CFLAGS) src/receiver.cpp -o receiver.o

sender.o:	headers/sender.h src/sender.cpp
	$(CC) $(CFLAGS) src/sender.cpp -o sender.o

router.o:	headers/router.h src/router.cpp
	$(CC) $(CFLAGS) src/router.cpp -o router.o


clean:
	rm -f *.o *.out *.mp3