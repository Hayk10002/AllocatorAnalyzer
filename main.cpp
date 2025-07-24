#include <iostream>
#include <string>
#include <cstdlib>
#include <ranges>

std::string replaceAll(
    const std::string& str,
    const std::string& from,
    const std::string& to)
{
    std::string result;
    bool first = true;

    for (auto&& part : std::views::split(str, std::views::all(from))) {
        if (!first) {
            result += to;
        }
        first = false;
        result += std::string(part.begin(), part.end());
    }

    return result;
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " --analyzer=<analyzer_command> --allocator=<allocator_command>\n";
        return 1;
    }

    std::string analyzer, allocator;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--analyzer=", 0) == 0) {
            analyzer = arg.substr(11); // Extract value after "--analyzer="
        } else if (arg.rfind("--allocator=", 0) == 0) {
            allocator = arg.substr(12); // Extract value after "--allocator="
        }
    }

    if (analyzer.empty() || allocator.empty()) {
        std::cerr << "Both --analyzer and --allocator arguments are required.\n";
        return 1;
    }
    std::string command = allocator + " | " + analyzer;     
#ifdef _WIN32
    command = replaceAll(command, "/", "\\");
#endif
    std::cout << "Executing command: " << command << "\n";
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Error executing command: " << command << "\n";
        return 1;
    }

    return 0;
}