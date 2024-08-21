#include "config.hpp"
#include "util.hpp"
#include <iostream>

int main() {
    if (!parseConfigFile("default.conf")) {
        std::cout << "Failed to parse config file" << std::endl;
        return 1;
    }

    // Print the parsed configuration for verification
    printConfig(servers);

    return 0;
}
