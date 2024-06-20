
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include <vector>


enum Phase 
{
    SLOW_START, 
    CONGESTION_AVOIDANCE, 
    FAST_RETRANSMITION, 
    FAST_RECOVERY
};

class Client
{
private:
    Phase phase;
    std::vector<std::string> data;

    int mySocket;
    bool connectionEstablished;
    int ssthresh;
    int cwnd;

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
    void generateData(int);
};