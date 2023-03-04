#include <iostream>
#include <cstring>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string input;

    // Loop till i type quit
    // Loop until user types "quit"
    do {
        input  = "Enter 'quit' to quit the application";
        std::cout << input << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), [](char c){ return std::tolower(c); });
    }
    while (input != "quit");
    ///

    return 0;
}
