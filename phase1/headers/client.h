
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class Client
{
private:
    int mySocket;
    bool connectionEstablished;

    void sendSyn();
    bool receiveSynAck();
    void sendAck();
    char buffer[1024] = {0};

public:
    Client();
    ~Client();

    void performHandshake();
    void sendData();
    void terminateConnection();
};