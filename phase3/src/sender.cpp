#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define WSIZE 4

using namespace std;

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    std::ifstream file("src/music.mp3", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // Send
    char buffer[1536] = {0};
    for (int i=1; file.read(buffer, sizeof(buffer)); i++)
    {
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        memset(buffer, 0, sizeof(buffer));

        if (!(i%WSIZE))
        {
            socklen_t serverSock = sizeof(serverAddr);
            recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &serverSock);
            std::cout << "received ack\n";
            memset(buffer, 0, sizeof(buffer));
        }
    }
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "File sent successfully" << std::endl;


    close(sockfd);

    return 0;
}
