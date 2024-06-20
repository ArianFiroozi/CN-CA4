#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#define PORT_SENDER 8080
#define PORT_RECEIVER 8081
#define WSIZE 4

using namespace std;

int main() {
    // Create a UDP socket to receive from sender
    int sockfd_sender = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_sender < 0) {
        std::cerr << "Error creating sender socket" << std::endl;
        return 1;
    }

    struct sockaddr_in senderAddr, receiverAddr;
    senderAddr.sin_family = AF_INET;
    senderAddr.sin_port = htons(PORT_SENDER);
    senderAddr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd_sender, (struct sockaddr*)&senderAddr, sizeof(senderAddr));
    int sockfd_receiver = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_receiver < 0) {
        std::cerr << "Error creating receiver socket" << std::endl;
        return 1;
    }
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT_RECEIVER);
    receiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Receive the MP3 file from the sender and forward it
    char buffer[1536] = {0};

    socklen_t addrLen = sizeof(senderAddr);
    for (int i=1;true;i++)
    {
        recvfrom(sockfd_sender, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, &addrLen);
        std::cout << "forwarding new packet\n";
        sendto(sockfd_receiver, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
        memset(buffer, 0, sizeof(buffer));

        if (!(i%WSIZE))
        {   
            socklen_t recAddrLen = sizeof(receiverAddr);
            recvfrom(sockfd_receiver, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, &recAddrLen);
            std::cout << "forwarding ack\n";
            sendto(sockfd_sender, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, sizeof(senderAddr));
            memset(buffer, 0, sizeof(buffer));
        }
    }

    std::cout << "File received successfully" << std::endl;

    close(sockfd_sender);
    close(sockfd_receiver);

    return 0;
}
