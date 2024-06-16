#include "../headers/client.h"

using namespace std;

int main() {
    Client client;  

    client.performHandshake();

    for (int i=0;i<5;i++)
        client.sendData();
    client.terminateConnection();

    return 0;
}

Client::Client()
{
    connectionEstablished=false;

    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(mySocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void Client::sendSyn()
{
    send(mySocket, "SYN", 42, 0);
    cout << "SYN sent\n";
}

bool Client::receiveSynAck()
{
    read(mySocket, buffer, 1024);
    if (strcmp(buffer, "SYN-ACK") == 0)
    {
        cout << "SYN-ACK received\n";
        return true;
    }
    return false;
}

void Client::sendAck()
{
    send(mySocket, "ACK", 42, 0);
    cout << "ACK sent, connection established\n";
    connectionEstablished=true;
}

void Client::performHandshake()
{
    sendSyn();
    if (receiveSynAck())
        sendAck();
    else
        cout << "Syn-Ack could not be received!\n";
}

void Client::sendData()
{
    if (!connectionEstablished)
    {
        cout << "No connection found!\n";
        return;
    }

    string data = "Hello, server!";
    send(mySocket, data.c_str(), data.size(), 0);
    cout << "Packet sent\n";

    memset(buffer, 0, 1024);
    read(mySocket, buffer, 1024);

    cout << "received ACK:" << buffer << endl;
}

Client::~Client()
{
    close(mySocket);
}

void Client::terminateConnection()
{
    if (!connectionEstablished)
    {
        cout << "No connection found!\n";
        return;
    }

    string data = "termination";
    send(mySocket, data.c_str(), data.size(), 0);
    cout << "Termination sent!\n";
}
