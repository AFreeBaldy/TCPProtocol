//
// Created by User on 3/5/2023.
//

#ifndef TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H
#define TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H


#include <winsock.h>
#include <ws2tcpip.h>
#include <memory>
#include <thread>

#define DEFAULT_INCOMING_DATA_BUFFER_SIZE 1024
#define SUCCESS 0
#define BASE_ERROR 1
class TCP_Connection {
private:
    bool isBinded = false;
    SOCKET sock;
    std::thread thread;

    [[nodiscard]] static int listenAndHandleConnections(SOCKET sock) {
        auto otherSocket = INVALID_SOCKET;
        // TODO fill the last 2 arguements of accept with proper values: SO
        struct sockaddr_in clientSockAddrIn{};
        int addrLen = sizeof(clientSockAddrIn);
        auto* ptr = reinterpret_cast<sockaddr *>(&clientSockAddrIn);
        otherSocket = accept(sock, ptr, &addrLen);
        if (otherSocket == INVALID_SOCKET) {
            return BASE_ERROR;
        }
        char incomingDataBuffer[DEFAULT_INCOMING_DATA_BUFFER_SIZE];
        int recvResults;
        do {
            recvResults = recv(otherSocket, incomingDataBuffer, sizeof(incomingDataBuffer), 0);
            //TODO do something with the data you received
        } while(recvResults > 0);
        if (recvResults == SOCKET_ERROR) {
            // Error handle
        }

        return SUCCESS;
    }


public:

    int bindTo(const int port) {

        // Initialize the Winsock
        WSADATA wsaData;
        int wsaStartupResults = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaStartupResults >= WSABASEERR) {
            // Log an error
            // exit function
            return BASE_ERROR;
        }

        sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            return BASE_ERROR;
        }
        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(port);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        auto* ptr = reinterpret_cast<sockaddr *>(&sockaddrIn);

        if (bind(sock, ptr, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return BASE_ERROR;
        }

        if (listen(sock, SOMAXCONN ) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return BASE_ERROR;
        }

        std::unique_ptr<std::thread> threadPtr;
        threadPtr = std::make_unique<std::thread>(listenAndHandleConnections, sock);
        thread.swap(*threadPtr);
        isBinded = true;
        return 0;
    }

    int unbind() {
        int socketClosed = closesocket(sock);
        if (socketClosed) {
            return 1;
        }
        isBinded = false;
        return 0;
    }
};

#endif //TCPPROTOCOL_TCP_WINDOWS_CONNECTION_H
