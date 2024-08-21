#include "util.hpp"
#include <iostream>

void printConfig(const std::vector<ServerConfig>& servers) {
    for (size_t i = 0; i < servers.size(); ++i) {
        const ServerConfig& server = servers[i];
        std::cout << "Server " << i + 1 << ":\n";
        std::cout << "  Listen: " << server.listen << "\n";
        std::cout << "  Server Name: " << server.server_name << "\n";
        std::cout << "  Host: " << server.host << "\n";
        std::cout << "  Root: " << server.root << "\n";
        std::cout << "  Index: " << server.index << "\n";
        for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it) {
            std::cout << "  Error Page " << it->first << ": " << it->second << "\n";
        }
        for (size_t j = 0; j < server.locations.size(); ++j) {
            const Location& loc = server.locations[j];
            std::cout << "  Location " << j + 1 << ":\n";
            std::cout << "    Path: " << loc.path << "\n";
            std::cout << "    Autoindex: " << (loc.autoindex ? "on" : "off") << "\n";
            std::cout << "    Index: " << loc.index << "\n";
            std::cout << "    Return: " << loc.return_path << "\n";
            for (size_t k = 0; k < loc.allow_methods.size(); ++k) {
                std::cout << "    Allow Method: " << loc.allow_methods[k] << "\n";
            }
            for (size_t k = 0; k < loc.cgi_path.size(); ++k) {
                std::cout << "    CGI Path: " << loc.cgi_path[k] << "\n";
            }
            for (size_t k = 0; k < loc.cgi_ext.size(); ++k) {
                std::cout << "    CGI Ext: " << loc.cgi_ext[k] << "\n";
            }
        }
    }
}
