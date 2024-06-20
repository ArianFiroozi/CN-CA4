#include "../headers/receiver.h"
#include <chrono>

using namespace std;

#define PORT 8081
#define WSIZE 4

int main() {

    
    Receiver receiver;
    receiver.createFile();

    auto start = chrono::high_resolution_clock::now();
    
    receiver.receiveData();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout <<"transport time was: " << duration.count() << endl;

    return 0;
}

Receiver::Receiver()
{
    mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket < 0)
        cerr << "error creating socket\n";

    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(PORT);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    bind(mySocket, (struct sockaddr*)&myAddr, sizeof(myAddr));

    window.push_back(window1);
    window.push_back(window2);
    window.push_back(window3);
    window.push_back(window4);
}

void Receiver::createFile()
{
    file = ofstream("received_music.mp3", ios::binary);
    if (!file)
        cerr << "Error opening output file" << endl;
}

void Receiver::receiveData()
{
    int recSize;
    bool packCorrect = true;
    socklen_t addrLen = sizeof(routerAddr);

    for (int i=1;(recSize=recvfrom(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&routerAddr, &addrLen))>0;i++)
    {
        cout << "new packet:" << buffer[1535] << endl;

        for (int j=0;j<1536;j++)
            window[(i+3)%4][j] = buffer[j];

        try { 
            if (stoi(string(1, buffer[1535])) != i%4)
            {
                packCorrect = false;
                cout << "packet corrupt!" <<endl;
            }
        }
        catch(invalid_argument e)
        {
            //terminated
            break;
        }

        memset(buffer, 0, sizeof(buffer));

        if (!(i%WSIZE))
        {
            if (packCorrect)
            {
                for (auto packet:window)
                    file.write(packet, recSize-1);
            }
            cout << "sending ack\n";
            buffer[0] = packCorrect ? '0' : '1';
            buffer[1] = '\0';
            sendto(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&routerAddr, sizeof(routerAddr));
            memset(buffer, 0, sizeof(buffer));
            packCorrect=true;

        }
    }

    cout << "file received, terminating...\n";
}

Receiver::~Receiver()
{
    file.close();
    close(mySocket);
}
