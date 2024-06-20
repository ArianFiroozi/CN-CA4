#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include <vector>

class Router
{
private:
    int senderPort;
    int receiverPort;

public:
    Router();
    ~Router();
};