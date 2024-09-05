#include "config.hpp"
#include "util.hpp"
#include <iostream>
#include "WebServer.hpp"
#include "HttpResponse.hpp"
#include "ErrorHandel.hpp"

int main() {
    // Parse the config file to get server configurations
    if (!parseConfigFile("default.conf")) {
        std::cout << "Failed to parse config file" << std::endl;
        return 1; // Exit if configuration parsing fails
    }

    printConfig(servers);

    // Start servers based on the parsed configurations
    for (std::vector<ServerConfig>::iterator it = servers.begin(); it != servers.end(); ++it) {
        WebServer server(*it);
        server.start();
    }

    return 0;
}
