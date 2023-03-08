//
// Created by User on 3/2/2023.
//

#include <memory>
#include "tcp_packet.h"
#include "tcp_windows_connection.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 10000

int onPacketReceived(TCP_Packet& packet, sockaddr_in clientAddrInfo) {


}

struct Server {
    std::thread thread;
    std::shared_ptr<TCP_Connection> connection;
};

struct Server startServer() {
    // Set up connection
    //std::shared_ptr<TCP_Connection> connection = std::make_shared<TCP_Connection>();
    //int (*onPacketReceived)(TCP_Packet& packet, sockaddr_in clientAddrInfo);
    //std::thread thread(TCP_Connection::listenOn, connection, SERVER_IP, SERVER_PORT, onPacketReceived);
    //struct Server server;
    //server.thread.swap(thread);
    //server.connection = connection;
    //return server;
}

int stopServer(struct Server& server) {
    int code = server.connection->stopListening();
    if (code != 0) return -1;
    server.thread.join();
    return 0;
}

