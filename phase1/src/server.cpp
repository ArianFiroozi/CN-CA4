#include "../headers/server.h"

using namespace std;

int main() {
    Server server;
    server.start();

    return 0;
}

Server::Server()
{
    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
    bind(mySocket, (struct sockaddr*)&server_address, sizeof(server_address));
}

Server::~Server()
{
    for (auto& thread : threads)
        thread.join();
}

void Server::start()
{
    listen(mySocket, 5);

    while (true) {
        int client_socket = accept(mySocket, NULL, NULL);
        threads.push_back(std::thread(handleClient, client_socket));
    }
}


void handleClient(int client_socket) {
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