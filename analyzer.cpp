#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.starts_with("End")) break;
        if (line.starts_with("Allocated")) std::cout << line << std::endl;
    }
    return 0;
}