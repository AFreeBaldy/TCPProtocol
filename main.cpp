#include <iostream>
#include <cstring>
#include <memory>
#include "tcp_connection.h"
#include "tcp_windows_connection.h"

struct Server {
    std::thread thread;
    std::shared_ptr<TCP_Connection> connection;
};

struct Client {
    std::thread threadIn;
    std::thread threadOut;
    std::shared_ptr<TCP_Connection> connection;
};

struct Client startClient();
struct Server startServer();
int stopClient(struct Client& client);
int stopServer(struct Server& server);
int sendDataFromClient(const std::shared_ptr<char*[] data);

int main() {
    //struct Server server = startServer();
    struct Client client = startClient();

    std::string input;
    do {
        send
        input  = "Enter 'quit' to quit the application";
        std::cout << input << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), [](char c){ return std::tolower(c); });
    }
    while (input != "q");
    stopClient(client);
    //stopServer(server);

}
