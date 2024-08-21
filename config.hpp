#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

// Define data structures
struct Location {
    std::string path;
    std::vector<std::string> allow_methods;
    bool autoindex;
    std::string index;
    std::string return_path;
    std::string root;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ext;
};

struct ServerConfig {
    int listen;
    std::string server_name;
    std::string host;
    std::string root;
    std::string index;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;
};

extern std::vector<ServerConfig> servers;

bool parseConfigFile(const std::string& filename);
bool validateConfig(const ServerConfig& server);

#endif // CONFIG_HPP
