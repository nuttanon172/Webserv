#include "util.hpp"
#include <iostream>
#include <map>

std::string getErrorPage(int error_code) {
    switch (error_code) {
        case 404:
            return "<html><body><h1>404 Not Found 2</h1></body></html>";
        case 500:
            return "<html><body><h1>500 Internal Server Error</h1></body></html>";
        // Add more cases as needed
        default:
            return "<html><body><h1>Unknown Error</h1></body></html>";
    }
}


std::string getContentType(const std::string& uri) {

    size_t dot = uri.find_last_of('.');
    if (dot != std::string::npos) {
        std::string ext = uri.substr(dot);
        std::map<std::string, std::string>::const_iterator it = mime_types.find(ext);
       // Logger::logRequest("content-type", ext);

        if (it != mime_types.end()) {
            return it->second;
        }
    }
    return "application/octet-stream"; // Default binary type
}


// // Function implementation to return the reason phrase for a given status code
// std::string getReasonPhrase(int status_code) {
//     switch (status_code) {
//         case 200: return "OK";
//         case 404: return "Not Found";
//         case 500: return "Internal Server Error";
//         // Add more status codes as needed
//         default: return "Unknown Status Code";
//     }
// }

// std::string getReasonPhrase(int status_code) {
//     std::map<int, std::string>::const_iterator it = status_reason_map.find(status_code);
//     if (it != status_reason_map.end()) {
//         return it->second;
//     }
//     return "Unknown Status";
// }

std::map<int, std::string>createStatusMap() {
    std::map<int, std::string> m;
    m.insert(std::make_pair(200, "OK"));
     m.insert(std::make_pair(201, "Created"));
      m.insert(std::make_pair(204, "No Content"));
       m.insert(std::make_pair(301, "Moved Permanently"));
        m.insert(std::make_pair(302, "Found"));
         m.insert(std::make_pair(304, "Not Modified"));
          m.insert(std::make_pair(400, "Bad Request"));
           m.insert(std::make_pair(200, "OK"));

           return m;

    // {401, "Unauthorized"},
    // {403, "Forbidden"},
    // {404, "Not Found"},
    // {500, "Internal Server Error"},
    // {502, "Bad Gateway"},
    // {503, "Service Unavailable"}
};

// Function implementation
std::map<std::string, std::string> createMimeTypesMap() {
    std::map<std::string, std::string> m;
    m.insert(std::make_pair(".html", "text/html"));
    m.insert(std::make_pair(".htm", "text/html"));
    m.insert(std::make_pair(".css", "text/css"));
    m.insert(std::make_pair(".js", "application/javascript"));
    m.insert(std::make_pair(".json", "application/json"));
    m.insert(std::make_pair(".png", "image/png"));
    m.insert(std::make_pair(".jpg", "image/jpeg"));
    m.insert(std::make_pair(".jpeg", "image/jpeg"));
    m.insert(std::make_pair(".gif", "image/gif"));
    m.insert(std::make_pair(".txt", "text/plain"));
    return m;
}

// Define mime_types as a static const map
const std::map<std::string, std::string> mime_types = createMimeTypesMap();

void printConfig(const std::vector<ServerConfig>& servers) {
    for (size_t i = 0; i < servers.size(); ++i) {
        const ServerConfig& server = servers[i];
        std::cout << "Server " << i + 1 << ":\n";
       // std::cout << "  Listen: " << server.listen << "\n";
        std::cout << "  Server Name: " << server.server_name << "\n";
        std::cout << "  Host: " << server.host << "\n";
        std::cout << "  Root: " << server.root << "\n";
     //   std::cout << "  Index: " << server.index << "\n";
        
        // Iterate through error pages
        for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it) {
            std::cout << "  Error Page " << it->first << ": " << it->second << "\n";
        }

        // Iterate through locations
        for (size_t j = 0; j < server.locations.size(); ++j) {
            const Location& loc = server.locations[j];
            std::cout << "  Location " << j + 1 << ":\n";
            std::cout << "    Path: " << loc.path << "\n";
            std::cout << "    Autoindex: " << (loc.autoindex ? "on" : "off") << "\n";
            //std::cout << "    Index: " << loc.index << "\n";
            std::cout << "    Return: " << loc.return_path << "\n";

            // Iterate through allow_methods map
            for (std::map<std::string, bool>::const_iterator method_it = loc.allow_methods.begin(); method_it != loc.allow_methods.end(); ++method_it) {
                std::cout << "    Allow Method: " << method_it->first << " is " << (method_it->second ? "enabled" : "disabled") << "\n";
            }

            // Iterate through CGI paths
            for (size_t k = 0; k < loc.cgi_path.size(); ++k) {
                std::cout << "    CGI Path: " << loc.cgi_path[k] << "\n";
            }

            // Iterate through CGI extensions
            for (size_t k = 0; k < loc.cgi_ext.size(); ++k) {
                std::cout << "    CGI Ext: " << loc.cgi_ext[k] << "\n";
            }
        }
    }
}
