#include "../headers/client.h"

using namespace std;

int main() {
    Client client;  

    client.performHandshake();

    client.generateData();
    client.sendData();

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

    cwnd = 1;
    ssthresh = 5;
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

void Client::generateData(int size)
{
    for (int i=0;i<size; i++)
    {
        data.push_back("this is a packet!");
    }
}

void Client::sendData()
{
    if (!connectionEstablished)
    {
        cout << "No connection found!\n";
        return;
    }

    int lastPacketSent = 0;
    int lastAck = -1;
    int drops = 0;

    for (int i=0;i<cwnd;i++)
    {
        send(mySocket, data[i].c_str(), data.size(), 0);
        cout << "Packet sent\n";
        lastPacketSent++;
    }

    while (true)
    {
        memset(buffer, 0, 1024);
        read(mySocket, buffer, 1024);

        cout << "received ACK:" << buffer << endl;
        int newAck = buffer[4] - 48;

        //TODO:implement partial ack
        if (newAck == lastAck+1 || ++drops < 3)
        {
            lastAck++;
            if (phase = SLOW_START)
            {
                if (cwnd*2<=ssthresh)
                {
                    for (int i=0;i<cwnd;i++)
                    {
                        send(mySocket, data[lastPacketSent].c_str(), data.size(), 0);
                        cout << "Packet sent\n";
                        lastPacketSent++;
                    }
                    cwnd *= 2;
                }
                else
                    phase = CONGESTION_AVOIDANCE;
            }
            if (phase = CONGESTION_AVOIDANCE)
            {
                send(mySocket, data[lastPacketSent].c_str(), data.size(), 0);
                cout << "Packet sent\n";
                lastPacketSent++;
                cwnd++;
            }
        }
        else {
            lastAck = newAck;
            drops = 0;
            phase = CONGESTION_AVOIDANCE;
            ssthresh = cwnd / 2;
            lastPacketSent = newAck;
            cwnd /= 2;

            send(mySocket, data[lastPacketSent].c_str(), data.size(), 0);
            cout << "Packet sent\n";
            lastPacketSent++;
        }
    }

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
