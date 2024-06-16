#include <iostream>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void handleClient(int);

class Server
{
private:
    int mySocket;
    std::vector<std::thread> threads;


public:
    Server();
    ~Server();

    void start();
};