//
// Created by User on 3/5/2023.
//

#ifndef TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H
#define TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H


#include <winsock.h>
#include <ws2tcpip.h>
#include <memory>
#include <thread>
#include <iostream>
#include "tcp_packet.h"

#define DEFAULT_INCOMING_DATA_BUFFER_SIZE 1024
#define SUCCESS 0
#define G_BASE_ERROR 1


class TCP_Connection {
private:
    SOCKET sock;
    std::thread thread;
    bool keepListening = true;

    int listenOn(void(*onPacketRecieved)(TCP_Packet tcpPacket, sockaddr_in clientInfo)) {
        // Incoming connections will be routed to this socket for handling
        auto otherSocket = INVALID_SOCKET;

        // This struct is filled with client address information
        struct sockaddr_in clientSockAddrIn{};
        int addrLen = sizeof(clientSockAddrIn);
        auto *ptr = reinterpret_cast<sockaddr *>(&clientSockAddrIn);

        // listen for incoming connections
        do {
            // Accept a connection and send it to otherSocket for handling
            otherSocket = accept(sock, ptr, &addrLen);
            if (otherSocket == INVALID_SOCKET) {
                //TODO potential error logging in the future
                std::cout << "Socket failed to create" << "\n";
                return G_BASE_ERROR;
            }

            // Buffer that will hold the data being recieved
            char incomingDataBuffer[DEFAULT_INCOMING_DATA_BUFFER_SIZE];
            int amountOfBytesReceived;

            do {
                amountOfBytesReceived = recv(otherSocket, incomingDataBuffer, sizeof(incomingDataBuffer), 0);

                // Copy the first 20 bytes into the tcp fixed size header
                struct TCP_Header_Fixed_Size fixedTCPHeader{};
                memcpy(&fixedTCPHeader, incomingDataBuffer, sizeof(fixedTCPHeader));

                // Create the full size tcp header
                struct TCP_Header tcpHeader;
                tcpHeader.headerFixedSize = fixedTCPHeader;
                // Perform a bitwise and operator then shift it right 12 bits to get first 4 bits which is the Data Offset
                short dataOffset = (tcpHeader.headerFixedSize.infoBits & 0xF000) >> 12;
                memcpy(tcpHeader.options, incomingDataBuffer + 20, (dataOffset * 4) - 20);

                // calculate the padding
                int padding = (amountOfBytesReceived - sizeof tcpHeader) % 4;

                // Construct tcp packet
                struct TCP_Packet tcpPacket;
                tcpPacket.tcpHeader = tcpHeader;
                int offset = sizeof tcpPacket.tcpHeader + padding;
                memcpy(tcpPacket.data, incomingDataBuffer + offset, amountOfBytesReceived - offset);

                onPacketRecieved(tcpPacket, clientSockAddrIn);
            } while (amountOfBytesReceived > 0 && keepListening);
            // Log any errors
            if (amountOfBytesReceived == SOCKET_ERROR) {
                //TODO add error logging functionality in the future
                return G_BASE_ERROR;
            }
        } while(keepListening);

        return SUCCESS;
    }


public:

    int unbind() {
        int socketClosed = closesocket(sock);
        if (socketClosed) {
            return 1;
        }
        isBinded = false;
        return 0;
    }

    int send(const char (&buffer)[DEFAULT_INCOMING_DATA_BUFFER_SIZE], const char ip[4], const unsigned short port) {
        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return G_BASE_ERROR;
        }

    }

    int listenOn(const std::string ip, const unsigned short port, void(*onPacketRecieved)(TCP_Packet tcpPacket, sockaddr_in clientInfo)) {
        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return G_BASE_ERROR;
        }

        sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            return G_BASE_ERROR;
        }
        // Construct ip from strig
        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &sockaddrIn.sin_addr) == -1) {
            WSACleanup();
            closesocket(sock);
            return G_BASE_ERROR;
        }

        auto* ptr = reinterpret_cast<sockaddr *>(&sockaddrIn);

        if (bind(sock, ptr, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return G_BASE_ERROR;
        }

        if (listen(sock, SOMAXCONN ) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return G_BASE_ERROR;
        }

        listenOn(onPacketRecieved);

    }


};

#endif //TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H
