//
// Created by User on 3/2/2023.
//


#ifndef TCPPROTOCOL_TCP_CONNECTION_H
#define TCPPROTOCOL_TCP_CONNECTION_H

#include <string>
#include <utility>
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <ws2tcpip.h>

struct TCP_Packet;

struct bindToReturned {
    int sock;
    std::thread thread1;
};


class TCP_CONNECTION {
private:
    static void listenOn(int sock, void (*func)(TCP_Packet& tcpPacket)) {
        char buffer[1024];

    }

public:
    bindToReturned bindTo(char& ip, char& port, void (*onReceived)(TCP_Packet& tcpPacket)) {
        int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (sock < 0) {
            std::cerr << "socket failed";
            exit(EXIT_FAILURE);
        }
        struct sockaddr_in sockaddrIn;
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(12345);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (struct sockaddr*)&sockaddrIn, sizeof(sockaddrIn)) == -1) {
            std::cerr << "Error binding socket\n";
            closesocket(sock);
            exit(EXIT_FAILURE);
        }
        std::thread listeningThread(listenOn, sock, onReceived);

        return {sock, std::move(listeningThread)};
    }
};

#endif //TCPPROTOCOL_TCP_CONNECTION_H
