#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

// Define data structures
struct Location
{
    std::string path;
    std::map<std::string, bool> allow_methods; //--> maps string {GET , TRUE} post delete
    bool autoindex;
    std::vector<std::string> index;
    std::string root;
    int client_max_body_size;
    std::map<int, std::string> return_path;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ext;
};

struct ServerConfig
{
    std::vector<int> listen_ports;
    std::string server_name;
    std::string host;
    std::string root;
    int client_max_body_size;
    std::vector<std::string> index;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;
    std::map<std::string, std::map<int, std::string>> location_return_path;
};

bool parseConfigFile(const std::string &filename, std::vector<ServerConfig> &servers);
bool validateConfig(const ServerConfig &server);
int stringToInt(const std::string &str);
void printServerConfig(const ServerConfig &server);

#endif // CONFIG_HPP
