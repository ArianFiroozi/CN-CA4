#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char const *filename = "src/music.mp3";
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    // {
    //     std::cerr << "Invalid address!" << std::endl;
    //     return 1;
    // }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return 1;
    }

    // Send file contents
    char buffer[1536] = {0};
    while (file.read(buffer, sizeof(buffer)))
    {
        send(sock, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }
    send(sock, buffer, strlen(buffer), 0);

    std::cout << "File sent successfully" << std::endl;

    // Close socket and file
    close(sock);
    file.close();
    return 0;
}