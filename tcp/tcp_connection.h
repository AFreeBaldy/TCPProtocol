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
#include "tcp_packet.h"

#endif

struct TCP_Packet;
struct TCP_Header_Fixed_Size;

struct BindToReturned {
    int sock;
    std::thread thread;
};


class TCP_CONNECTION {
private:
    int sock;
    std::unique_ptr<std>()std::thread thread;

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
    static void test() {

    }
public:
    std::atomic<bool> joinRequested = false;

    BindToReturned bindTo(const char ip[], const char port[], void (*onReceived)(TCP_Packet& tcpPacket)) {
        sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
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
        // TODO dont forget to delete the pointer
        thread.swap( *(new std::thread(listenOn, sock, onReceived, std::ref(joinRequested))));

        return {sock, std::move(listeningThread)};
    }
};

#endif //TCPPROTOCOL_TCP_CONNECTION_H
