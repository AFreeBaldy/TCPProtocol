//
// Created by User on 3/2/2023.
//

#include "tcp_packet.h"
#include "tcp_connection.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "10000"

struct BindToReturned;
void onPacketReceived(TCP_Packet& packet) {


}

BindToReturned testServer() {
    // Setup connection
    TCP_CONNECTION connection;
    // Bind to a port
    struct BindToReturned bindToReturned = connection.bindTo(SERVER_IP, SERVER_PORT, onPacketReceived);
    return bindToReturned;
}