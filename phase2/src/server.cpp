#include "../headers/server.h"

using namespace std;

int main() {
    Server server;

    std::thread t([&server]() {
        server.start();
    });

    t.join();
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
    listen(mySocket, 42);

    while (true) {
        int clientSocket = accept(mySocket, NULL, NULL);
        threads.push_back(std::thread(handleClient, clientSocket));
    }
}


void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int lastPacketReceived = -1;
    read(clientSocket, buffer, 1024);
    if (strcmp(buffer, "SYN") == 0) {
        std::cout << "SYN received\n";

        send(clientSocket, "SYN-ACK", 7, 0);
        std::cout << "SYN-ACK sent\n";

        memset(buffer, 0, 1024);
        read(clientSocket, buffer, 1024);
        if (strcmp(buffer, "ACK") == 0) {
            std::cout << "ACK received, connection established\n";

            while (true)
            {
                memset(buffer, 0, 1024);
                read(clientSocket, buffer, 1024);
                if (strcmp(buffer, "termination")==0)
                    break;
                std::cout << "Received data: " << buffer << "\n";

                if (lastPacketReceived + 1 == stoi(buffer))
                    lastPacketReceived++;
                
                string ackStr = "";
                ackStr.append("ACK");
                ackStr.append(to_string(lastPacketReceived+1));
                send(clientSocket, ackStr.c_str() , ackStr.size(), 0);
            }
        }
    }

    close(clientSocket);
}