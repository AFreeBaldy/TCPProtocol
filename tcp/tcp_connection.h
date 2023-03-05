//
// Created by User on 3/2/2023.
//


#ifndef TCPPROTOCOL_TCP_CONNECTION_H
#define TCPPROTOCOL_TCP_CONNECTION_H

#include <string>
#include <utility>
#include <iostream>
#include <thread>
#ifdef _WIN32
#include "tcp_windows_include.h"
#else
#include "tcp_unix_include.h"
#endif
#include "tcp_packet.h"

struct TCP_Packet;
struct TCP_Header_Fixed_Size;

class TCP_CONNECTION {
private:
    static void listenOn(int sock, void (*func)(TCP_Packet& tcpPacket), std::atomic<bool>& joinRequested) {
        char buffer[1024];
        // listen on for incoming data
        while (!joinRequested) {
            int data_received = recv(sock, buffer, sizeof buffer, 0);
            if (data_received < 0) {
                // I want to error
                std::cerr << "Data failed to receive";
            } else {
                // Handle the data here
                // Put the first 20 bytes into a fixed tcp header
                struct TCP_Header_Fixed_Size tcpHeaderFixedSize{};
                memcpy(&tcpHeaderFixedSize, &buffer, 20);
            }
        }


    }
public:
    TCP_CONNECTION() = default;
    std::thread thread;
    SOCKET ListenSocket = INVALID_SOCKET;
    std::atomic<bool> joinRequested = false;



    void bindTo(const char ip[], const int port, void (*onReceived)(TCP_Packet& tcpPacket)) {
        // Initialize Winsock
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
            exit(EXIT_FAILURE);
        }

        ListenSocket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (ListenSocket < 0) {
            WSACleanup();
            std::cerr << "socket failed";
            exit(EXIT_FAILURE);
        }
        struct sockaddr_in sockaddrIn;
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(port);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        if (bind(ListenSocket, (struct sockaddr*)&sockaddrIn, sizeof(sockaddrIn)) == -1) {
            std::cerr << "Error binding socket\n";
            closesocket(ListenSocket);
            exit(EXIT_FAILURE);
        }
        std::unique_ptr<std::thread> threadPtr;
        threadPtr = std::make_unique<std::thread>(listenOn, ListenSocket, onReceived, std::ref(joinRequested));
        thread.swap(*threadPtr);
    }


};

#endif //TCPPROTOCOL_TCP_CONNECTION_H
