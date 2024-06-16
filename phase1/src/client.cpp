#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    send(client_socket, "SYN", 4, 0);
    std::cout << "SYN sent\n";

    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    if (strcmp(buffer, "SYN-ACK") == 0) {
        std::cout << "SYN-ACK received\n";

        send(client_socket, "ACK", 4, 0);
        std::cout << "ACK sent, connection established\n";

        std::string data = "Hello, server!";
        send(client_socket, data.c_str(), data.size(), 0);
    }

    close(client_socket);

    return 0;
}