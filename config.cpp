#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cctype>  


// Function to check if a line starts with a certain keyword
bool startsWith(const std::string& line, const std::string& keyword) {
    return trim(line).find(keyword) == 0;
}


bool checkServerAndLocationBlocks(const std::string& filename) {
    std::ifstream configFile(filename.c_str());
    if (!configFile) {
        std::cerr << "Unable to open configuration file: " << filename << std::endl;
        return false;
    }

    std::string line;
    bool insideServerBlock = false;
    bool insideLocationBlock = false;
    bool serverStarted = false;
    bool locationStarted = false;
    int lineNum = 0;
    int openBracesCount = 0;

    while (std::getline(configFile, line)) {
        lineNum++;
        std::string trimmedLine = trim(line);

        // Ignore empty lines
        if (trimmedLine.empty()) continue;

        // Check for server block start
        if (!insideServerBlock && startsWith(trimmedLine, "server")) {
            serverStarted = true;
        }

        // Check for opening of server block (brace '{')
        if (serverStarted && trimmedLine.find('{') != std::string::npos) {
            insideServerBlock = true;
            serverStarted = false;
            openBracesCount++;
        }

        // If inside server block, check for location block start
        if (insideServerBlock && !insideLocationBlock && startsWith(trimmedLine, "location")) {
            locationStarted = true;
        }

        // Check for opening of location block (brace '{')
        if (locationStarted && trimmedLine.find('{') != std::string::npos) {
            insideLocationBlock = true;
            locationStarted = false;
            openBracesCount++;
        }

        // Allow for a case where `location` is split across two lines
        if (locationStarted && trimmedLine.find('{') == std::string::npos) {
            std::getline(configFile, line);
            lineNum++;
            trimmedLine = trim(line);
            if (trimmedLine == "{") {
                insideLocationBlock = true;
                locationStarted = false;
                openBracesCount++;
            } else {
                std::cerr << "Error: Missing opening brace for location block, line " << lineNum << std::endl;
                return false;
            }
        }

        // Check for closing of location or server block
        if (insideLocationBlock || insideServerBlock) {
            if (trimmedLine == "}") {
                openBracesCount--;
                if (insideLocationBlock) {
                    insideLocationBlock = false;
                } else if (insideServerBlock && openBracesCount == 0) {
                    insideServerBlock = false;
                }
            }
        }

        // Error detection: if any config directive appears outside of the server or location blocks
        if (!insideServerBlock && !insideLocationBlock && !serverStarted && !locationStarted) {
            if (trimmedLine != "}") { // Allowing braces to close blocks
                std::cerr << "Error: Directive '" << trimmedLine 
                          << "' must be inside a server block, line " << lineNum << std::endl;
                return false;
            }
        }

        // Error detection: location block directive outside server block
        if (!insideServerBlock && insideLocationBlock) {
            std::cerr << "Error: Location block must be inside a server block, line " << lineNum << std::endl;
            return false;
        }
    }

    // Final checks for missing closing braces
    if (insideServerBlock) {
        std::cerr << "Error: Missing closing brace for server block" << std::endl;
        return false;
    }
    if (insideLocationBlock) {
        std::cerr << "Error: Missing closing brace for location block" << std::endl;
        return false;
    }

    configFile.close();
    return true; // No errors found
}



bool checkBracesOnSeparateLine(const std::string& filename) {
    std::ifstream configFile(filename.c_str());
    if (!configFile) {
        std::cerr << "Unable to open configuration file: " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineNum = 0;             // To track line numbers for error reporting

    while (std::getline(configFile, line)) {
        lineNum++;
        
        // Remove leading/trailing spaces for cleaner checks
        std::string trimmedLine;
        size_t firstChar = line.find_first_not_of(" \t");
        if (firstChar != std::string::npos)
            trimmedLine = line.substr(firstChar);
        
        // Check if a brace is on the same line as other characters
        if (trimmedLine.find('{') != std::string::npos && trimmedLine.length() > 1) {
            std::cerr << "Error: '{' must be on its own line at line " << lineNum << std::endl;
            return false;
        }
        
        if (trimmedLine.find('}') != std::string::npos && trimmedLine.length() > 1) {
            std::cerr << "Error: '}' must be on its own line at line " << lineNum << std::endl;
            return false;
        }
    }

    configFile.close();
    return true; // If no issues were found
}


bool isNotSpace(int ch) {
    return !std::isspace(ch);
}

// Trim function
std::string trim(const std::string& s) {
    std::string result = s;

    result.erase(result.begin(), std::find_if(result.begin(), result.end(), isNotSpace));
    result.erase(std::find_if(result.rbegin(), result.rend(), isNotSpace).base(), result.end());

    return result;
}

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

       if (!checkBracesOnSeparateLine(filename)) 
     {
         std::cerr << "{ or } must to stay on new line" << filename << std::endl;
        return false;
     }

     if (!checkServerAndLocationBlocks(filename)) {
        
        std::cout << "Configuration file failed location block checks." << std::endl;
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

        // Remove semicolon if present
        if (!line.empty() && line[line.size() - 1] == ';')
        {
            line = line.substr(0, line.size() - 1);
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

        // Check for a lone "{" line, which is not allowed before `server` or `location`
        if (line == "{")
        {
            std::cerr << "Error: Unmatched '{' found on a new line." << std::endl;
            return false;
        }

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        // Handle opening "server" block with "{" on the same line or next line
        if (key == "server")
        {
            if (in_server_block)
            {
                std::cerr << "Error: Nested server block found." << std::endl;
                return false;
            }

            std::string next_token;
            iss >> next_token;

            if (next_token == "{")
            {
                in_server_block = true;
                server_block_count++;
                current_server = ServerConfig(); // Reset the server config
            }
            else if (next_token.empty())
            {
                // Check for "{" on the next line
                std::getline(config_file, line);
                std::istringstream next_iss(line);
                next_iss >> next_token;

                if (next_token == "{")
                {
                    in_server_block = true;
                    server_block_count++;
                    current_server = ServerConfig(); // Reset the server config
                }
                else
                {
                    std::cerr << "Error: Missing '{' after 'server' keyword." << std::endl;
                    return false;
                }
            }
            else
            {
                std::cerr << "Error: Unexpected token '" << next_token << "' after 'server' keyword." << std::endl;
                return false;
            }
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

            iss >> current_location.path;
            if (current_location.path[0] != '/')
                current_location.path = "/" + current_location.path;
            std::string next_token;
            iss >> next_token;

            if (next_token == "{")
            {
                in_location_block = true;
                location_block_count++;
            }
            else if (next_token.empty())
            {
                // Check for "{" on the next line
                std::getline(config_file, line);
                std::istringstream next_iss(line);
                next_iss >> next_token;

                if (next_token == "{")
                {
                    in_location_block = true;
                    location_block_count++;
                }
                else
                {
                    std::cerr << "Error: Missing '{' after 'location' keyword." << std::endl;
                    return false;
                }
            }
            else
            {
                std::cerr << "Error: Unexpected token '" << next_token << "' after 'location' keyword." << std::endl;
                return false;
            }

            // Initialize HTTP methods
            current_location.allow_methods["GET"] = true;
            current_location.allow_methods["POST"] = true;
            current_location.allow_methods["DELETE"] = true;
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
            // Other key handling (like listen, server_name, etc.)
            if (key == "listen")
            {
                std::string port_str;
                while (iss >> port_str)
                {
                    if (!isNumber(port_str))
                    {
                        std::cerr << "Error: Port '" << port_str << "' is not a valid number." << std::endl;
                        return false;
                    }

                    int parsed_port = stringToInt(port_str);

                    if (!isValidrange(parsed_port, 8000, 10000))
                    {
                        std::cerr << "Error: Port '" << parsed_port << "' is out of valid range (8000-9000)." << std::endl;
                        return false;
                    }

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
            else if (key == "cgi_path")
            {
                std::string path;
                while (iss >> path)
                {
                    if (in_location_block)
                    {
                        current_location.cgi_path.push_back(path);
                    }
                }
            }
            else if (key == "cgi_ext")
            {
                std::string path;
                while (iss >> path)
                {
                    if (in_location_block)
                    {
                        current_location.cgi_ext.push_back(path);
                    }
                }
            }
            else if (key == "client_max_body_size")
            {
                std::string bodysize;
                iss >> bodysize;
                int intbodysize = stringToInt(bodysize);

                if (intbodysize <= 0)
                {
                    std::cerr << "Error: Invalid client_max_body_size value." << std::endl;
                    return false;
                }

                if (!isValidrange(intbodysize, 1, 3000001))
                {
                    std::cerr << "Error: Size '" << intbodysize << "' is out of valid range." << std::endl;
                    return false;
                }

                if (in_location_block)
                {
                    current_location.client_max_body_size = intbodysize;
                }
                else
                {
                    current_server.client_max_body_size = intbodysize;
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
                
                current_location.allow_methods["GET"] = false;
                current_location.allow_methods["POST"] = false;
                current_location.allow_methods["DELETE"] = false;
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
            else if (key == "return")
            {
                int status_code;
                std::string url;

                // Check if a status code is provided
                if (!(iss >> status_code))
                {
                    // No status code provided, default to 302 (temporary redirect)
                    status_code = 302;

                    // Make sure to extract the URL after skipping the missing status code
                    iss.clear();
                    iss >> url;
                }
                else
                {
                    // Status code provided, get the URL
                    iss >> url;
                }

                if (url.empty())
                {
                    std::cerr << "Error: 'return' directive missing URL." << std::endl;
                    return false;
                }

                current_location.return_path[status_code] = url;
                current_server.location_return_path.insert(std::make_pair(current_location.path, current_location.return_path));
                /*std::cout << "path: " << current_server.location_return_path.begin()->first << '\n';
                std::cout << "code: " << current_server.location_return_path.begin()->second.begin()->first << '\n';
                std::cout << "path redirect: " << current_server.location_return_path.begin()->second.begin()->second << '\n';
                std::cout << "-------\n";
                std::cout << "path: " << current_location.path << '\n';
                std::cout << "code: " << status_code << '\n';
                std::cout << "path redirect: " << current_location.return_path[status_code] << '\n';*/
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
        std::cout << "Path: " << loc.path << std::endl;
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

        std::cout << "    Return Path:" << std::endl;
        for (std::map<int, std::string>::const_iterator it = loc.return_path.begin(); it != loc.return_path.end(); ++it)
        {
            std::cout << "    Code " << it->first << ": " << it->second << std::endl;
        }

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
