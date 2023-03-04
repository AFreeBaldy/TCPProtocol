#include <iostream>
#include <cstring>
#include "tcp_connection.h"

BindToReturned testServer();
struct BindToReturned;

int main() {
    struct BindToReturned bindToReturned = testServer();


    std::string input;
    do {
        input  = "Enter 'quit' to quit the application";
        std::cout << input << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), [](char c){ return std::tolower(c); });
    }
    while (input != "quit");
    ///
    closesocket(bindToReturned.sock);
    bindToReturned.thread.join();
    return 0;
}
