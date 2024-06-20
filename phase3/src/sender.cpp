#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>

#define PORT 8080
#define WSIZE 4

using namespace std;

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    std::ifstream file("src/music.mp3", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // Send
    vector<char*> window;
    char window1[1536];
    char window2[1536];
    char window3[1536];
    char window4[1536];
    window.push_back(window1);
    window.push_back(window2);
    window.push_back(window3);
    window.push_back(window4);

    char buffer[1536] = {0};
    for (int i=1; file.read(buffer, sizeof(buffer)-1); i++)
    {
        buffer[1535] = i%4==1?'1':i%4==2?'2':i%4==3?'3':'0';

        for (int j=0;j<1536;j++)
            window[(i+3)%4][j] = buffer[j];

        sendto(sockfd, window[(i+3)%4], sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        memset(buffer, 0, sizeof(buffer));
        

        if (!(i%WSIZE))
        {
            socklen_t serverSock = sizeof(serverAddr);
            recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &serverSock);
            std::cout << "received ack:" <<buffer<<endl;
            // cout<<buffer[1535]<<window[0][1535]<<window[1][1535]<<window[2][1535]<<window[3][1535]<<endl;

            while (stoi(string(1, buffer[0])))
            {
                for (int j=0;j<WSIZE;j++)
                    sendto(sockfd, window[j], 1536, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                
                memset(buffer, 0, sizeof(buffer));
                recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &serverSock);
            }
            memset(buffer, 0, sizeof(buffer));
            window.clear();
        }
    }
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "File sent successfully" << std::endl;


    close(sockfd);

    return 0;
}
