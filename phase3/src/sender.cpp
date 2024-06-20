#include "../headers/sender.h"

#define PORT 8080
#define WSIZE 4

using namespace std;

int main() {
    Sender sender;
    sender.openFile();
    sender.sendData();

    return 0;
}

Sender::Sender()
{
    mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket < 0)
        cerr << "error creating socket\n";

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    receiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    window.push_back(window1);
    window.push_back(window2);
    window.push_back(window3);
    window.push_back(window4);
}

void Sender::openFile()
{
    file = ifstream("src/music.mp3", ios::binary);
    if (!file)
        cerr << "Error opening file" << endl;
}

void Sender::sendData()
{
    for (int i=1; file.read(buffer, sizeof(buffer)-1); i++)
    {
        buffer[1535] = i%4==1?'1':i%4==2?'2':i%4==3?'3':'0';

        for (int j=0;j<1536;j++)
            window[(i+3)%4][j] = buffer[j];

        sendto(mySocket, window[(i+3)%4], sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
        memset(buffer, 0, sizeof(buffer));
        

        if (!(i%WSIZE))
        {
            socklen_t receiverSock = sizeof(receiverAddr);
            recvfrom(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, &receiverSock);
            cout << "received ack:" <<buffer<<endl;

            while (stoi(string(1, buffer[0])))
            {
                for (int j=0;j<WSIZE;j++)
                    sendto(mySocket, window[j], 1536, 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
                
                memset(buffer, 0, sizeof(buffer));
                recvfrom(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, &receiverSock);
            }
            memset(buffer, 0, sizeof(buffer));
            window.clear();
        }
    }
    sendto(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));

    cout << "file sent successfully, terminating...\n";
}

Sender::~Sender()
{
    close(mySocket);
    file.close();    
}
