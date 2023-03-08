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
#include <utility>
#include <vector>
#include "tcp_packet.h"

#define DEFAULT_INCOMING_DATA_BUFFER_SIZE 1024
#define SUCCESS 0
#define G_BASE_ERROR 1
#define TCP_SERVER_TYPE 1
#define TCP_CLIENT_TYPE 2



class TCP_Connection {
private:
    SOCKET sock;
    bool keepListening = true;
    const std::string ip;
    const unsigned short port;
    std::mutex dataOutMutex;
    const short type;

    int receiveData(int (*onDataIn)(TCP_Packet packet, sockaddr_in clientAddrInfo)) {
        // This struct is filled with client address information
        struct sockaddr_in clientSockAddrIn{};
        int addrLen = sizeof(clientSockAddrIn);
        auto *ptr = reinterpret_cast<sockaddr *>(&clientSockAddrIn);


        // Needed for polling

        WSAPOLLFD mySocketPollFD = {sock, POLLIN, 0};
        // listen for incoming connections
        do {
            int results = WSAPoll(&mySocketPollFD, 1, 5000);
            if (results == SOCKET_ERROR) {

            } else if (results == 0) {
                // Socket timed out
            } else {
                // Accept a connection and send it to otherSocket for handling
                char incomingDataBuffer[DEFAULT_INCOMING_DATA_BUFFER_SIZE];
                ssize_t amountOfBytesReceived = recvfrom(sock, incomingDataBuffer, sizeof incomingDataBuffer, 0, ptr, &addrLen);
                if (amountOfBytesReceived == 0) continue;
                if (amountOfBytesReceived == SOCKET_ERROR) continue;
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

                (*onDataIn)(tcpPacket, clientSockAddrIn);
            }
        } while(keepListening);

        return SUCCESS;
    }

    static int conn(std::shared_ptr<TCP_Connection> connection, int (*onDataIn)(TCP_Packet packet, sockaddr_in clientAddrInfo)) {
        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return G_BASE_ERROR;
        }

        connection->sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (connection->sock == INVALID_SOCKET) {
            WSACleanup();
            return G_BASE_ERROR;
        }

        // Construct ip from strig
        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(connection->port);
        if (inet_pton(AF_INET, connection->ip.c_str(), &sockaddrIn.sin_addr) == -1) {
            WSACleanup();
            closesocket(connection->sock);
            return G_BASE_ERROR;
        }
        auto* ptr = reinterpret_cast<sockaddr *>(&sockaddrIn);

        if (connect(connection->sock, ptr, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            closesocket(connection->sock);
            WSACleanup();
            return G_BASE_ERROR;
        }

        connection->receiveData(onDataIn);
    }
    static int bind_(std::shared_ptr<TCP_Connection> connection, int (*onDataIn)(TCP_Packet packet, sockaddr_in clientAddrInfo)) {
        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return G_BASE_ERROR;
        }

        connection->sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (connection->sock == INVALID_SOCKET) {
            WSACleanup();
            return G_BASE_ERROR;
        }

        // Construct ip from strig
        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(connection->port);
        if (inet_pton(AF_INET, connection->ip.c_str(), &sockaddrIn.sin_addr) == -1) {
            WSACleanup();
            closesocket(connection->sock);
            return G_BASE_ERROR;
        }
        auto* ptr = reinterpret_cast<sockaddr *>(&sockaddrIn);

        if (connect(connection->sock, ptr, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            closesocket(connection->sock);
            WSACleanup();
            return G_BASE_ERROR;
        }

        connection->receiveData(onDataIn);
    }

    int sendData(char* data) {
        // Get window size


    }
    int p_listenOn(int (*onPR)(TCP_Packet& packet, sockaddr_in clientAddrInfo)) {


    }


public:
    const std::vector<char> dataOut;
    TCP_Connection(std::string ip, const unsigned short port, const short type) :ip(std::move(ip)), port(port), type(type) {
        sock = INVALID_SOCKET;
    }

    int sendData(const SOCKET socket, const std::shared_ptr<char*> data, const int size) {
        int results = send(sock, *data, size, 0);
        return results;

    }

    static int listenOn(std::shared_ptr<TCP_Connection> connection, int (*onPR)(TCP_Packet& packet, sockaddr_in clientAddrInfo)) {
        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return G_BASE_ERROR;
        }

        connection->sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (connection->sock == INVALID_SOCKET) {
            WSACleanup();
            return G_BASE_ERROR;
        }
        // Construct ip from strig
        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(connection->port);
        if (inet_pton(AF_INET, connection->ip.c_str(), &sockaddrIn.sin_addr) == -1) {
            WSACleanup();
            closesocket(connection->sock);
            return G_BASE_ERROR;
        }

        auto* ptr = reinterpret_cast<sockaddr *>(&sockaddrIn);

        if (bind(connection->sock, ptr, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            closesocket(connection->sock);
            WSACleanup();
            return G_BASE_ERROR;
        }

        connection->p_listenOn(onPR);

    }

    int stopListening() {
        keepListening = false;
        WSACleanup();
        int socketClosed = closesocket(sock);
        if (socketClosed) {
            return 1;
        }
        return 0;
    }

};

#endif //TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H
