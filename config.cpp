#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

std::vector<ServerConfig> servers;

bool parseConfigFile(const std::string& filename) {
    std::ifstream config_file(filename.c_str());
    if (!config_file.is_open()) {
        std::cout << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    ServerConfig current_server;
    Location current_location;
    bool in_server_block = false;
    bool in_location_block = false;

    while (std::getline(config_file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "server") {
            in_server_block = true;
            if (!current_server.locations.empty()) {
                if (!validateConfig(current_server)) {
                    return false;
                }
                servers.push_back(current_server);
                current_server = ServerConfig();
            }
        } else if (key == "location") {
            in_location_block = true;
            if (!current_location.path.empty()) {
                current_server.locations.push_back(current_location);
                current_location = Location();
            }
            iss >> current_location.path;
        } else if (key == "listen") {
            iss >> current_server.listen;
        } else if (key == "server_name") {
            iss >> current_server.server_name;
        } else if (key == "host") {
            iss >> current_server.host;
        } else if (key == "root") {
            if (in_location_block) {
                iss >> current_location.root;
            } else {
                iss >> current_server.root;
            }
        } else if (key == "index") {
            if (in_location_block) {
                iss >> current_location.index;
            } else {
                iss >> current_server.index;
            }
        } else if (key == "error_page") {
            int code;
            std::string path;
            iss >> code >> path;
            current_server.error_pages[code] = path;
        } else if (key == "allow_methods") {
            std::string method;
            while (iss >> method) {
                current_location.allow_methods.push_back(method);
            }
        } else if (key == "autoindex") {
            std::string value;
            iss >> value;
            current_location.autoindex = (value == "on");
        } else if (key == "return") {
            iss >> current_location.return_path;
        } else if (key == "cgi_path") {
            std::string path;
            while (iss >> path) {
                current_location.cgi_path.push_back(path);
            }
        } else if (key == "cgi_ext") {
            std::string ext;
            while (iss >> ext) {
                current_location.cgi_ext.push_back(ext);
            }
        } else if (key == "}") {
            if (in_location_block) {
                in_location_block = false;
                current_server.locations.push_back(current_location);
                current_location = Location();
            } else if (in_server_block) {
                in_server_block = false;
                if (!validateConfig(current_server)) {
                    return false;
                }
                servers.push_back(current_server);
                current_server = ServerConfig();
            }
        }
    }

    if (!current_server.locations.empty()) {
        if (!validateConfig(current_server)) {
            return false;
        }
        servers.push_back(current_server);
    }

    config_file.close();
    return true;
}


// Function to check if a directory exists (POSIX compliant)
bool directoryExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false; // Directory doesn't exist
    } else if (info.st_mode & S_IFDIR) {
        return true;  // Path is a directory
    }
    return false; // Path exists but is not a directory
}

bool validateConfig(const ServerConfig& server) {
    // Check if server_name contains a colon ":"
    if (server.server_name.find(':') != std::string::npos) {
        std::cerr << "Error: server_name \"" << server.server_name << "\" contains invalid character ':'\n";
        return false;
    }

    // Validate each location
    for (std::vector<Location>::const_iterator it = server.locations.begin(); it != server.locations.end(); ++it) {
        std::string full_path = server.root + it->path;
        if (!directoryExists(full_path)) {
            std::cerr << "Error: Location root \"" << full_path << "\" does not exist\n";
            return false;
        }
    }

    return true;
}


