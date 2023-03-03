//
// Created by User on 3/2/2023.
//

#define SERVER_IP "127.0.0.1"
void onPacketRecieved() {

}

void testServer() {
    // Setup connection
    TCP_Connection connection;
    // Bind to a port
    connection.bindTo(SERVER_IP, SERVER_PORT, onPacketRecieved);
}