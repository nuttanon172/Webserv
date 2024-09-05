#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <string>
#include <netinet/in.h>
#include "config.hpp"
#include "Logger.hpp"

class WebServer {
private:
    int server_socket;
    struct sockaddr_in server_addr;
    ServerConfig config;

public:
    // Constructor
    WebServer(const ServerConfig& config);

    // Copy constructor
    WebServer(const WebServer& other);

    // Assignment operator
    WebServer& operator=(const WebServer& other);

    // Destructor
    ~WebServer();

    // Methods
    void start();
    void handleRequest(int client_socket);
    void handleError(int client_socket, int error_code);
    std::string parseRequestedResource(const std::string& http_request);
};

#endif // WEBSERVER_HPP
