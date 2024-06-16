// #include "headers/server.h"
// #include "headers/client.h"

// int main()
// {
//     Server server;
//     Client client1;

//     std::thread serverThread([&server]() {
//         server.start();
//     });

//     client1.performHandshake();
//     std::thread client1Thread([&client1]() {
//         client1.sendData();
//         usleep(100);client1.sendData();
//         usleep(100);client1.sendData();
//         usleep(100);
//         client1.sendData();
//     });


//     client1Thread.join();
//     serverThread.join();
// }