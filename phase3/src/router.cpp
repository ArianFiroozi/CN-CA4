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

    // Bind the sender socket to the port
    bind(sockfd_sender, (struct sockaddr*)&senderAddr, sizeof(senderAddr));

    // Create a UDP socket to send to receiver
    int sockfd_receiver = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_receiver < 0) {
        std::cerr << "Error creating receiver socket" << std::endl;
        return 1;
    }

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT_RECEIVER);
    receiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Receive the MP3 file from the sender
    char buffer[1536] = {0};
    int n;
    socklen_t addrLen = sizeof(senderAddr);
    while ((n = recvfrom(sockfd_sender, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, &addrLen)) > 0)
    {
        std::cout << "newPacket\n";
        sendto(sockfd_receiver, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
        memset(buffer, 0, sizeof(buffer));
    }

    std::cout << "File received successfully" << std::endl;

    close(sockfd_sender);
    close(sockfd_receiver);

    return 0;
}
