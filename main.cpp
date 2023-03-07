#include <iostream>
#include <cstring>
#include <memory>
#include "tcp_connection.h"
#include "tcp_windows_connection.h"

struct Server {
    std::thread thread;
    std::shared_ptr<TCP_Connection> connection;
};

struct Server startServer();
int stopServer(struct Server& server);

int main() {
    struct Server server = startServer();


    std::string input;
    do {
        input  = "Enter 'quit' to quit the application";
        std::cout << input << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), [](char c){ return std::tolower(c); });
    }
    while (input != "q");
    stopServer(server);

}
