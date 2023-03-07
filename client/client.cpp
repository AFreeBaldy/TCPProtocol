//
// Created by User on 3/6/2023.
//
#include <iostream>
#include <memory>
#include "tcp_windows_connection.h"

#define CLIENT_PORT 11000
#define CLIENT_IP "127.0.0.1"

struct Client {
    std::shared_ptr<TCP_Connection> connection;
    std::thread threadIn;
    std::thread threadOut;
};

struct Client startClient() {
    struct Client client;
    client.connection = std::make_shared<TCP_Connection>(CLIENT_PORT, CLIENT_IP);

}

int sendDataFromClient(struct Client& client) {

}

int stopClient(struct Client& client) {

}