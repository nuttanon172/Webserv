#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <set>
#include <map>
#include <vector>

// Define data structures
struct Location
{
    std::string path;
    std::map<std::string, bool> allow_methods; //--> maps string {GET , TRUE} post delete
    bool autoindex;
    std::vector<std::string> index;
    std::string root;
    size_t client_max_body_size;
    std::map<int, std::string> return_path;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ext;
};

struct ServerConfig
{
    bool autoindex;
    std::vector<int> listen_ports;
    std::string server_name;
    std::string host;
    std::string root;
    size_t client_max_body_size;
    std::vector<std::string> index;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;
    std::map<std::string, std::map<int, std::string> > location_return_path;
};

bool parseConfigFile(const std::string &filename, std::vector<ServerConfig> &servers);
bool validateConfig(const ServerConfig &server);
int stringToInt(const std::string &str);
size_t stringToST(const std::string &str);
void printServerConfig(const ServerConfig &server);
std::string trim(const std::string &s);

#endif // CONFIG_HPP
