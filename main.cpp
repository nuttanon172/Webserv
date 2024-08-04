#include <iostream>
#include <fstream>
#include <sstream>
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
                servers.push_back(current_server);
                current_server = ServerConfig();
            }
        }
    }

    config_file.close();
    return true;
}

int main() {
    if (!parseConfigFile("default.conf")) {
        std::cout << "Failed to parse config file" << std::endl;
        return 1;
    }

    // Print the parsed configuration for verification
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

    return 0;
}
