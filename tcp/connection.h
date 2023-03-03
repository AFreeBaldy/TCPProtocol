//
// Created by User on 3/2/2023.
//


#ifndef TCPPROTOCOL_CONNECTION_H
#define TCPPROTOCOL_CONNECTION_H

#include <string>
#include <utility>

struct TCP_Packet {
    unsigned short sourcePort;
    unsigned short destinationPort;
    unsigned  int sequenceNumber;
    unsigned  int acknowledgementNumber;

};

class Connection {

    explicit Connection(std::string localHost, std::string localPort, std::string serverHost, std::string serverPort)
        : localHost(std::move(localHost)), localPort(std::move(localPort)),  serverHost(std::move(serverHost)),  serverPort(std::move(serverPort)) {

    }

private:
    const std::string localHost;
    const std::string localPort;
    std::string serverHost;
    std::string serverPort;

public:
    bool connect() {

    }

    int disconnect() {

    }

    // Perform handshake
    // Returns 1 if something went wrong
    // 0 if handshake was successful
    int establishConnection() {
        // send
    }
};

#endif //TCPPROTOCOL_CONNECTION_H
