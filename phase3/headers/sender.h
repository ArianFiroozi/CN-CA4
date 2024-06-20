#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>

class Sender
{
private:
    int mySocket;
    char buffer[1536] = {0};
    struct sockaddr_in myAddr, routerAddr;
    std::vector<char*> window;
    char window1[1536];
    char window2[1536];
    char window3[1536];
    char window4[1536];
    std::ifstream file;
    struct sockaddr_in receiverAddr;

public:
    Sender();
    ~Sender();

    void openFile();
    void sendData();
};

#endif