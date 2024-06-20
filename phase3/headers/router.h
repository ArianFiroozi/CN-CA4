#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

class Router
{
private:
    int senderSocket, receiverSocket;
    struct sockaddr_in senderAddr, receiverAddr;
    char buffer[1536] = {0};

public:
    Router();
    ~Router();

    void forward();
};

#endif