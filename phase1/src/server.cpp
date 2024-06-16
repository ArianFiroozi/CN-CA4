#include <iostream>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void handle_client(int client_socket) {
    char buffer[1024] = {0};

    read(client_socket, buffer, 1024);
    if (strcmp(buffer, "SYN") == 0) {
        std::cout << "SYN received\n";

        send(client_socket, "SYN-ACK", 7, 0);
        std::cout << "SYN-ACK sent\n";

        memset(buffer, 0, 1024);
        read(client_socket, buffer, 1024);
        if (strcmp(buffer, "ACK") == 0) {
            std::cout << "ACK received, connection established\n";

            memset(buffer, 0, 1024);
            read(client_socket, buffer, 1024);
            std::cout << "Received data: " << buffer << "\n";
            send(client_socket, "Packet-ACK", 11, 0);
        }
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    std::vector<std::thread> threads;
    while (true) {
        int client_socket = accept(server_socket, NULL, NULL);
        threads.push_back(std::thread(handle_client, client_socket));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}