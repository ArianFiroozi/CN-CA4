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

int main() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Read the MP3 file
    std::ifstream file("src/music.mp3", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // Send file contents
    char buffer[1536] = {0};
    while (file.read(buffer, sizeof(buffer)))
    {
        // send(sock, buffer, sizeof(buffer), 0);
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        memset(buffer, 0, sizeof(buffer));
    }
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "File sent successfully" << std::endl;


    close(sockfd);

    return 0;
}
