#include "../headers/router.h"

#define PORT_SENDER 8080
#define PORT_RECEIVER 8081
#define WSIZE 4

using namespace std;

int main() 
{
    Router router;
    router.forward();
    
    return 0;
}

Router::Router()
{
    senderSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (senderSocket < 0)
        cerr << "error creating sender socket\n";

    senderAddr.sin_family = AF_INET;
    senderAddr.sin_port = htons(PORT_SENDER);
    senderAddr.sin_addr.s_addr = INADDR_ANY;
    bind(senderSocket, (struct sockaddr*)&senderAddr, sizeof(senderAddr));
    
    receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiverSocket < 0)
        cerr << "error creating receiver socket\n";
    
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT_RECEIVER);
    receiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void Router::forward()
{
    socklen_t addrLen = sizeof(senderAddr);
    for (int i=1;true;i++)
    {
        recvfrom(senderSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, &addrLen);
        cout << "forwarding new packet\n";
        sendto(receiverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
        memset(buffer, 0, sizeof(buffer));

        if (!(i%WSIZE))
        {   
            socklen_t recAddrLen = sizeof(receiverAddr);
            recvfrom(receiverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, &recAddrLen);
            cout << "forwarding ack\n";
            sendto(senderSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, sizeof(senderAddr));
            memset(buffer, 0, sizeof(buffer));
        }
    }

    cout << "file forwarder successfully\n";
}

Router::~Router()
{
    close(senderSocket);
    close(receiverSocket);
}
