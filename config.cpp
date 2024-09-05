#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stringToInt(const std::string& str) {
    std::istringstream iss(str);
    int num;
    iss >> num;

    // Check if the conversion succeeded
    if (iss.fail()) {
        std::cerr << "Error: invalid integer conversion from string \"" << str << "\"" << std::endl;
        return 0;  // You can decide how to handle the error
    }

    return num;
}

bool parseConfigFile(const std::string& filename, std::vector<ServerConfig> &servers) {
    std::ifstream config_file(filename.c_str());
    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    ServerConfig current_server;
    Location current_location;
    bool in_server_block = false;
    bool in_location_block = false;

    while (std::getline(config_file, line)) {
        // Remove comments (everything after '#')
        std::size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        // Manually trim whitespace from the line
        std::string::iterator it = line.begin();
        while (it != line.end() && std::isspace(*it)) {
            it = line.erase(it);
        }
        if (line.empty()) continue;
        std::string::reverse_iterator rit = line.rbegin();
        while (rit != line.rend() && std::isspace(*rit)) {
            rit = std::string::reverse_iterator(line.erase((++rit).base()));
        }

        // Remove semicolon if present
        if (!line.empty() && line[line.size() - 1] == ';') {
            line = line.substr(0, line.size() - 1);
        }

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

            // Initialize all HTTP methods to false
            current_location.allow_methods["GET"] = false;
            current_location.allow_methods["POST"] = false;
            current_location.allow_methods["DELETE"] = false;
            current_location.allow_methods["PUT"] = false;
            current_location.allow_methods["HEAD"] = false;
            current_location.allow_methods["OPTIONS"] = false;
            current_location.allow_methods["PATCH"] = false;
            // Add more methods if needed
        } else if (key == "listen") {
            std::string ports;
            iss >> ports;
            // Support both single and multiple ports on the same line
            std::istringstream port_stream(ports);
            std::string port;
            while (std::getline(port_stream, port, ',')) {
                current_server.listen_ports.push_back(stringToInt(port));
            }
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
        } else if (key == "client_max_body_size") {
            if (in_location_block) {
                iss >> current_location.client_max_body_size;
            } else {
                iss >> current_server.client_max_body_size;
            }
        } else if (key == "index") {
            std::string path;
            while (iss >> path) {
                if (in_location_block) {
                     current_location.index.push_back(path);
               // iss >> current_location.index;
                } else {
                     current_server.index.push_back(path);
                //iss >> current_server.index;
                }

               
            }
          
        } else if (key == "error_page") {
            int code;
            std::string path;
            iss >> code >> path;
            current_server.error_pages[code] = path;
        } else if (key == "allow_methods") {
            std::string method;
            while (iss >> method) {
                if (current_location.allow_methods.find(method) != current_location.allow_methods.end()) {
                    current_location.allow_methods[method] = true;  // Set the specified method to true
                } else {
                    std::cerr << "Warning: Unsupported HTTP method \"" << method << "\" found in config file.\n";
                }
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

// bool parseConfigFile(const std::string& filename) {
//     std::ifstream config_file(filename.c_str());
//     if (!config_file.is_open()) {
//         std::cerr << "Failed to open config file: " << filename << std::endl;
//         return false;
//     }

//     std::string line;
//     ServerConfig current_server;
//     Location current_location;
//     bool in_server_block = false;
//     bool in_location_block = false;

//     while (std::getline(config_file, line)) {
//         // Remove comments (everything after '#')
//         std::size_t comment_pos = line.find('#');
//         if (comment_pos != std::string::npos) {
//             line = line.substr(0, comment_pos);
//         }

//         // Manually trim whitespace from the line
//         std::string::iterator it = line.begin();
//         while (it != line.end() && std::isspace(*it)) {
//             it = line.erase(it);
//         }
//         if (line.empty()) continue;
//         std::string::reverse_iterator rit = line.rbegin();
//         while (rit != line.rend() && std::isspace(*rit)) {
//             rit = std::string::reverse_iterator(line.erase((++rit).base()));
//         }

//         // Remove semicolon if present
//         if (!line.empty() && line[line.size() - 1] == ';') {
//             line = line.substr(0, line.size() - 1);
//         }

//         std::istringstream iss(line);
//         std::string key;
//         iss >> key;

//         if (key == "server") {
//             in_server_block = true;
//             if (!current_server.locations.empty()) {
//                 if (!validateConfig(current_server)) {
//                     return false;
//                 }
//                 servers.push_back(current_server);
//                 current_server = ServerConfig();
//             }
//         } else if (key == "location") {
//             in_location_block = true;
//             if (!current_location.path.empty()) {
//                 current_server.locations.push_back(current_location);
//                 current_location = Location();
//             }
//             iss >> current_location.path;

//             // Initialize all HTTP methods to false
//             current_location.allow_methods["GET"] = false;
//             current_location.allow_methods["POST"] = false;
//             current_location.allow_methods["DELETE"] = false;
//             current_location.allow_methods["PUT"] = false;
//             current_location.allow_methods["HEAD"] = false;
//             current_location.allow_methods["OPTIONS"] = false;
//             current_location.allow_methods["PATCH"] = false;
//             // Add more methods if needed
//         } else if (key == "listen") {
//             iss >> current_server.listen;
//         } else if (key == "server_name") {
//             iss >> current_server.server_name;
//         } else if (key == "host") {
//             iss >> current_server.host;
//         } else if (key == "root") {
//             if (in_location_block) {
//                 iss >> current_location.root;
//             } else {
//                 iss >> current_server.root;
//             }
//         } else if (key == "index") {
//             std::string inpath;
//             while (iss >> inpath) {
//                 if (in_location_block) {
//                     current_location.index.push_back(inpath);
//                 //iss >> current_location.index;
//                  } else {
//                     current_server.index.push_back(inpath);
//                // iss >> current_server.index;
//                  }
//             }
            
//         } else if (key == "error_page") {
//             int code;
//             std::string path;
//             iss >> code >> path;
//             current_server.error_pages[code] = path;
//         } else if (key == "allow_methods") {
//             std::string method;
//             while (iss >> method) {
//                 if (current_location.allow_methods.find(method) != current_location.allow_methods.end()) {
//                     current_location.allow_methods[method] = true;  // Set the specified method to true
//                 } else {
//                     std::cerr << "Warning: Unsupported HTTP method \"" << method << "\" found in config file.\n";
//                 }
//             }
//         } else if (key == "autoindex") {
//             std::string value;
//             iss >> value;
//             current_location.autoindex = (value == "on");
//         } else if (key == "return") {
//             iss >> current_location.return_path;
//         } else if (key == "cgi_path") {
//             std::string path;
//             while (iss >> path) {
//                 current_location.cgi_path.push_back(path);
//             }
//         } else if (key == "cgi_ext") {
//             std::string ext;
//             while (iss >> ext) {
//                 current_location.cgi_ext.push_back(ext);
//             }
//         } else if (key == "}") {
//             if (in_location_block) {
//                 in_location_block = false;
//                 current_server.locations.push_back(current_location);
//                 current_location = Location();
//             } else if (in_server_block) {
//                 in_server_block = false;
//                 if (!validateConfig(current_server)) {
//                     return false;
//                 }
//                 servers.push_back(current_server);
//                 current_server = ServerConfig();
//             }
//         }
//     }

//     if (!current_server.locations.empty()) {
//         if (!validateConfig(current_server)) {
//             return false;
//         }
//         servers.push_back(current_server);
//     }

//     config_file.close();
//     return true;
// }

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
    if (server.server_name.find(':') != std::string::npos) {
        std::cerr << "Error: server_name \"" << server.server_name << "\" contains invalid character ':'\n";
        return false;
    }

    for (std::vector<Location>::const_iterator it = server.locations.begin(); it != server.locations.end(); ++it) {
        std::string full_path = "./" + server.root + it->path;
        if (!directoryExists(full_path)) {
            std::cerr << "Error: Location root \"" << full_path << "\" does not exist\n";
            return false;
        }
    }

    return true;
}

