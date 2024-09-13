#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool isValidrange(int port, int min, int max)
{
    // Check if the port is in the range 8000-9000
    return port >= min && port <= max;
}

bool isNumber(const std::string &s)
{
    if (s.empty())
    {
        return false;
    }

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (!std::isdigit(*it))
        {
            return false;
        }
    }
    return true;
}

int stringToInt(const std::string &str)
{
    std::istringstream iss(str);
    int num;
    iss >> num;

    // Check if the conversion succeeded
    if (iss.fail())
    {
        std::cerr << "Error: invalid integer conversion from string \"" << str << "\"" << std::endl;
        return 0; // You can decide how to handle the error
    }

    return num;
}

bool parseConfigFile(const std::string &filename, std::vector<ServerConfig> &servers)
{
    std::ifstream config_file(filename.c_str());
    if (!config_file.is_open())
    {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    ServerConfig current_server;
    Location current_location;
    bool in_server_block = false;
    bool in_location_block = false;
    int server_block_count = 0;
    int location_block_count = 0;

    while (std::getline(config_file, line))
    {
        // Remove comments (everything after '#')
        std::size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
        {
            line = line.substr(0, comment_pos);
        }

        // Manually trim whitespace from the line
        std::string::iterator it = line.begin();
        while (it != line.end() && std::isspace(*it))
        {
            it = line.erase(it);
        }
        if (line.empty())
            continue;
        std::string::reverse_iterator rit = line.rbegin();
        while (rit != line.rend() && std::isspace(*rit))
        {
            rit = std::string::reverse_iterator(line.erase((++rit).base()));
        }

        // Remove semicolon if present
        if (!line.empty() && line[line.size() - 1] == ';')
        {
            line = line.substr(0, line.size() - 1);
        }

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        // Handle opening "server" block
        if (key == "server")
        {
            if (in_server_block)
            {
                std::cerr << "Error: Nested server block found." << std::endl;
                return false;
            }

            std::string next_token;
            iss >> next_token;

            if (next_token != "{")
            {
                std::cerr << "Error: Missing '{' after 'server' keyword." << std::endl;
                return false;
            }

            in_server_block = true;
            server_block_count++;
            current_server = ServerConfig(); // Reset the server config
        }
        else if (key == "location")
        {
            if (!in_server_block)
            {
                std::cerr << "Error: 'location' block outside of 'server' block." << std::endl;
                return false;
            }
            if (in_location_block)
            {
                std::cerr << "Error: Nested location block found." << std::endl;
                return false;
            }

            std::string next_token;
            iss >> current_location.path >> next_token;

            if (next_token != "{")
            {
                std::cerr << "Error: Missing '{' after 'location' keyword." << std::endl;
                return false;
            }

            in_location_block = true;
            location_block_count++;

            // Initialize HTTP methods
            current_location.allow_methods["GET"] = false;
            current_location.allow_methods["POST"] = false;
            current_location.allow_methods["DELETE"] = false;
            current_location.allow_methods["PUT"] = false;
            current_location.allow_methods["HEAD"] = false;
            current_location.allow_methods["OPTIONS"] = false;
            current_location.allow_methods["PATCH"] = false;
        }
        else if (key == "}")
        {
            if (in_location_block)
            {
                in_location_block = false;
                location_block_count--;
                current_server.locations.push_back(current_location);
                current_location = Location(); // Reset location config
            }
            else if (in_server_block)
            {
                in_server_block = false;
                server_block_count--;
                if (!validateConfig(current_server))
                {
                    return false;
                }
                servers.push_back(current_server);
            }
            else
            {
                std::cerr << "Error: Unmatched closing '}' found." << std::endl;
                return false;
            }
        }
        else if (in_server_block)
        {
            // Parse other keys inside the server block
            if (key == "listen")
            {
                std::string port_str;
                while (iss >> port_str)
                {
                    // Check if the port string contains only numbers
                    if (!isNumber(port_str))
                    {
                        std::cerr << "Error: Port '" << port_str << "' is not a valid number." << std::endl;
                        return false;
                    }

                    int parsed_port = stringToInt(port_str); // Convert the string to an integer

                    // Validate the port is within the range 8000-9000
                    if (!isValidrange(parsed_port, 8000, 9000))
                    {
                        std::cerr << "Error: Port '" << parsed_port << "' is out of valid range (8000-9000)." << std::endl;
                        return false;
                    }

                    // Add the port to the server's listen_ports vector
                    current_server.listen_ports.push_back(parsed_port);
                }
            }
            else if (key == "server_name")
            {
                iss >> current_server.server_name;
            }
            else if (key == "host")
            {
                iss >> current_server.host;
            }
            else if (key == "root")
            {
                if (in_location_block)
                {
                    iss >> current_location.root;
                }
                else
                {
                    iss >> current_server.root;
                }
            }
            else if (key == "index")
            {
                std::string path;
                while (iss >> path)
                {
                    if (in_location_block)
                    {
                        current_location.index.push_back(path);
                    }
                    else
                    {
                        current_server.index.push_back(path);
                    }
                }
            }
            else if (key == "client_max_body_size")
            {
                std::string bodysize;
                iss >> bodysize;                         // Read the size into bodysize
                int intbodysize = stringToInt(bodysize); // Convert the size to an integer

                if (intbodysize <= 0)
                {
                    std::cerr << "Error: Invalid client_max_body_size value." << std::endl;
                    return false;
                }

                if (!isValidrange(intbodysize, 1, 3000001))
                {
                    std::cerr << "Error: Port '" << intbodysize << "' is out of valid range." << std::endl;
                    return false;
                }

                if (in_location_block)
                {
                    current_location.client_max_body_size = intbodysize; // Set for location
                }
                else
                {
                    current_server.client_max_body_size = intbodysize; // Set for server
                }
            }
            else if (key == "error_page")
            {
                int code;
                std::string path;
                iss >> code >> path;
                current_server.error_pages[code] = path;
            }
            else if (key == "allow_methods")
            {
                std::string method;
                while (iss >> method)
                {
                    if (current_location.allow_methods.find(method) != current_location.allow_methods.end())
                    {
                        current_location.allow_methods[method] = true;
                    }
                    else
                    {
                        std::cerr << "Warning: Unsupported HTTP method \"" << method << "\" found in config file.\n";
                    }
                }
            }
        }
    }

    // Final validation after loop
    if (server_block_count > 0)
    {
        std::cerr << "Error: Unclosed 'server' block found." << std::endl;
        return false;
    }

    if (location_block_count > 0)
    {
        std::cerr << "Error: Unclosed 'location' block found." << std::endl;
        return false;
    }

    config_file.close();
    return true;
}

void printServerConfig(const ServerConfig &server)
{
    std::cout << "Server Configuration:" << std::endl;

    std::cout << "Listen Ports: ";
    for (std::vector<int>::const_iterator it = server.listen_ports.begin(); it != server.listen_ports.end(); ++it)
    {
        std::cout << *it;
        if (it + 1 != server.listen_ports.end())
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Server Name: " << server.server_name << std::endl;
    std::cout << "Host: " << server.host << std::endl;
    std::cout << "Root: " << server.root << std::endl;
    std::cout << "Client Max Body Size: " << server.client_max_body_size << std::endl;

    std::cout << "Index: ";
    for (std::vector<std::string>::const_iterator it = server.index.begin(); it != server.index.end(); ++it)
    {
        std::cout << *it;
        if (it + 1 != server.index.end())
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Error Pages:" << std::endl;
    for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
    {
        std::cout << "  Code " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Locations:" << std::endl;
    for (std::vector<Location>::const_iterator loc_it = server.locations.begin(); loc_it != server.locations.end(); ++loc_it)
    {
        const Location &loc = *loc_it;
        std::cout << "  Path: " << loc.path << std::endl;
        std::cout << "    Allow Methods: ";
        for (std::map<std::string, bool>::const_iterator method_it = loc.allow_methods.begin(); method_it != loc.allow_methods.end(); ++method_it)
        {
            if (method_it->second)
            {
                std::cout << method_it->first << " ";
            }
        }
        std::cout << std::endl;

        std::cout << "    Autoindex: " << (loc.autoindex ? "on" : "off") << std::endl;

        std::cout << "    Index: ";
        for (std::vector<std::string>::const_iterator it = loc.index.begin(); it != loc.index.end(); ++it)
        {
            std::cout << *it;
            if (it + 1 != loc.index.end())
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        std::cout << "    Return Path: " << loc.return_path << std::endl;
        std::cout << "    Root: " << loc.root << std::endl;
        std::cout << "    Client Max Body Size: " << loc.client_max_body_size << std::endl;

        std::cout << "    CGI Path: ";
        for (std::vector<std::string>::const_iterator it = loc.cgi_path.begin(); it != loc.cgi_path.end(); ++it)
        {
            std::cout << *it;
            if (it + 1 != loc.cgi_path.end())
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        std::cout << "    CGI Ext: ";
        for (std::vector<std::string>::const_iterator it = loc.cgi_ext.begin(); it != loc.cgi_ext.end(); ++it)
        {
            std::cout << *it;
            if (it + 1 != loc.cgi_ext.end())
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

// Function to check if a directory exists (POSIX compliant)
bool directoryExists(const std::string &path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false; // Directory doesn't exist
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true; // Path is a directory
    }
    return false; // Path exists but is not a directory
}

bool validateConfig(const ServerConfig &server)
{
    if (server.server_name.find(':') != std::string::npos)
    {
        std::cerr << "Error: server_name \"" << server.server_name << "\" contains invalid character ':'\n";
        return false;
    }

    for (std::vector<Location>::const_iterator it = server.locations.begin(); it != server.locations.end(); ++it)
    {
        std::string full_path = "./" + server.root + it->path;
        if (!directoryExists(full_path))
        {
            std::cerr << "Error: Location root \"" << full_path << "\" does not exist\n";
            return false;
        }
    }

    return true;
}
