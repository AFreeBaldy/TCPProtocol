//
// Created by User on 3/6/2023.
//
#include <iostream>
#include <memory>
#include "tcp_windows_connection.h"

#define CLIENT_PORT 11000
#define CLIENT_IP "127.0.0.1"
#define MAX_CLIENT_DATA_TO_SEND 1024

struct Client {
    std::shared_ptr<TCP_Connection> connection;
    std::thread threadIn;
    std::thread threadOut;
};

int onDataReceived(TCP_Packet& packet, sockaddr_in serverInfo) {

}

struct Client startClient() {
    struct Client client;
    client.connection = std::make_shared<TCP_Connection>(CLIENT_IP, CLIENT_PORT);
    int (*onDR)(TCP_Packet& packet, sockaddr_in serverInfo) = &onDataReceived;
    std::thread threadIn(client.connection->listenOn, client.connection, onDR);
    client.threadIn.swap(threadIn);
    return client;
}

int sendDataFromClient(struct Client& client, const std::string ip, const unsigned short port, const std::shared_ptr<char[]> data) {
    return client.connection->send(ip, port, data);
}

int stopClient(struct Client& client) {
    client.connection->stopListening();
    client.threadIn.join();
    client.threadOut.join();
}