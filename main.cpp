#include <iostream>
#include <cstring>
#include <memory>
#include "tcp_connection.h"

class TCP_CONNECTION;
struct BindToReturned;
std::shared_ptr<TCP_CONNECTION> testServer();

int main() {
    std::shared_ptr<TCP_CONNECTION> connection = testServer();


    std::string input;
    do {
        input  = "Enter 'quit' to quit the application";
        std::cout << input << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), [](char c){ return std::tolower(c); });
    }
    while (input != "quit");
    ///
    //closesocket(connection->sock);
    connection->joinRequested = true;
    //connection->thread.join();
    WSACleanup();
    return 0;
}
