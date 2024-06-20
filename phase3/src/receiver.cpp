#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

using namespace std;

#define PORT 8081
#define WSIZE 4

int main() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Receive the MP3 file from the sender
    socklen_t addrLen = sizeof(clientAddr);


    // Save the received MP3 file
    std::ofstream file("received_music.mp3", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    // Receive file contents
    char buffer[1536] = {0};
    int n;
    for (int i=1;(n=recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen))>0;i++)
    {
        std::cout << "newPacket\n";
        file.write(buffer, n);
        memset(buffer, 0, sizeof(buffer));

        if (!(i%WSIZE))
        {
            std::cout << "sending ack\n";
            buffer[0] = 'a';
            buffer[1] = 'c';
            buffer[2] = '\0';
            sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
            memset(buffer, 0, sizeof(buffer));
        }
    }

    std::cout << "File received successfully" << std::endl;

    file.close();
    close(sockfd);

    return 0;
}
