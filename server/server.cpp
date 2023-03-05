//
// Created by User on 3/2/2023.
//

#include <memory>
#include "tcp_packet.h"
#include "tcp_connection.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 10000

void onPacketReceived(TCP_Packet& packet) {


}

std::shared_ptr<TCP_CONNECTION> testServer() {
    // Setup connection
    std::shared_ptr<TCP_CONNECTION> connection = std::make_shared<TCP_CONNECTION>();

    // Bind to a port
    connection->bindTo(SERVER_IP, SERVER_PORT, onPacketReceived);
    return connection;
}