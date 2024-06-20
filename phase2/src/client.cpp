#include "../headers/client.h"

using namespace std;

int main() {
    Client client;  

    client.performHandshake();

    client.generateData(100);
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

    cwnd = 4;
    ssthresh = 10;
    phase = SLOW_START;
    cout << "phase changed to slow start\n";
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
        data.push_back(to_string(i) + "this is a packet!");
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
    int lastAck = 0;
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
        buffer[0]='0';
        buffer[1]='0';
        buffer[2]='0';
        if (buffer[3] == 'P')
        {
            cout << "received partial ack, fast retransmit!\n";
            buffer[3] = '0';
            int newAck = stoi(buffer);
            send(mySocket, data[newAck].c_str(), data.size(), 0);
            drops--;
            // lastPacketSent++;
            // cwnd++;
            continue;
        }

        int newAck = stoi(buffer);

        if (newAck == int(data.size()))
        {
            terminateConnection();
            break;
        }

        
        if (newAck == lastAck+1 || ++drops < 3)
        {
            drops = (newAck == lastAck+1) ? 0 : drops; // reset if partial

            lastAck++;
            if (phase == SLOW_START)
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
                {
                    phase = CONGESTION_AVOIDANCE;
                    cout << "phase changed to congestion avoidance\n";
                }
            }
            if (phase == CONGESTION_AVOIDANCE)
            {
                send(mySocket, data[lastPacketSent].c_str(), data.size(), 0);
                cout << "Packet sent\n";
                lastPacketSent++;
                cwnd++;
            }
        }
        else {
            phase = FAST_RECOVERY;
            cout << "phase changed to fast recovery\n";

            lastAck = newAck;
            drops = 0;
            ssthresh = cwnd / 2;
            lastPacketSent = newAck;
            cwnd = ssthresh;

            send(mySocket, data[lastPacketSent].c_str(), data.size(), 0);
            cout << "Packet sent\n";
            lastPacketSent++;

            phase = CONGESTION_AVOIDANCE;
            cout << "phase changed to congestion avoidance\n";
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
