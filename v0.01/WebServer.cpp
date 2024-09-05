#include "WebServer.hpp"
#include "util.hpp"
#include "ErrorHandel.hpp"
#include "HttpResponse.hpp"
#include <cstdlib>   // For exit()
#include <cstring>   // For memset()
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>  // For close()
#include <iostream>  // For std::cout and std::endl
#include <fstream>
#include <sstream>
#include <string>

// Function to parse the requested resource from an HTTP request
std::string WebServer::parseRequestedResource(const std::string& http_request) {
    std::istringstream stream(http_request);
    std::string method, resource, version;

    // Extract the first line of the HTTP request (request line)
    stream >> method >> resource >> version;

    // If the request line is well-formed, return the resource
    if (!resource.empty()) {
        return resource;
    }

    // Return a default resource if parsing fails
    return "/";
}

// Function to read file content
std::string readFile(const std::string& file_path) {
    std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return contents.str();
}

// Constructor
WebServer::WebServer(const ServerConfig& config) : config(config) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        ErrorHandel error("Failed to create socket");
        error.printError();
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));  // Zero out server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(config.host.c_str()); // Use host from config
    //server_addr.sin_port = htons(config.listen); // Use port from config
    server_addr.sin_port = htons(8002); // Use port from config

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        ErrorHandel error("Bind failed");
        error.printError();
        exit(1);
    }

    listen(server_socket, 5);
}

// Destructor
WebServer::~WebServer() {
    close(server_socket);
}

// Methods
void WebServer::start() {
    //std::cout << "Server started on " << config.host << ":" << config.listen << std::endl;
    std::cout << "Server started on " << config.host << ": 8002" << std::endl;

    while (true) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            ErrorHandel error("Accept failed");
            error.printError();
            continue;
        }

        handleRequest(client_socket);
        close(client_socket);
    }
}

void WebServer::handleRequest(int client_socket) {
    char buffer[1024];
    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received < 0) {
        Logger::errorinfo("Failed to receive request");
        ErrorHandel error("Failed to receive request");
        error.printError();
        return;
    }
    buffer[received] = '\0';

    // // Simple request handling
    // HttpResponse response;
    // response.setBody("<html><body><h1>Hello, World!</h1></body></html>");

    // std::string httpResponse = response.generateResponse();
    // send(client_socket, httpResponse.c_str(), httpResponse.size(), 0);


    // Determine the requested resource
    std::string requested_resource = parseRequestedResource(buffer); // Implement this function

    // Read the file content
    std::string file_content = readFile("./main.cpp");
    //std::string file_content = readFile(requested_resource);

    Logger::logRequestinfo("127.0.0.1", requested_resource);


    HttpResponse response;
    if (file_content.empty()) {
        // File not found
        response.setStatusCode(404);
        response.setReasonPhrase(response.getReasonPhrase(404));
        response.setBody("<html><body><h1>404 Not Found 1</h1></body></html>");
        response.setHeader("Content-Type", "text/html");
        Logger::errorinfo("Failed to receive request");
    } else {
        // Success
        response.setStatusCode(200);
        response.setReasonPhrase(response.getReasonPhrase(200));
        response.setBody(file_content);
        response.setHeader("Content-Type", "text/html");//getContentType(requested_resource)); // Implement getContentType
    }

    std::string httpResponse = response.generateResponse();
    send(client_socket, httpResponse.c_str(), httpResponse.size(), 0);
}

void WebServer::handleError(int client_socket, int error_code) {
    HttpResponse response;
    response.setStatusCode(error_code);
    response.setReasonPhrase(response.getReasonPhrase(error_code));
    std::string error_page = getErrorPage(error_code); // Implement this function
    response.setBody(error_page);
    response.setHeader("Content-Type", "text/html");
    std::string httpResponse = response.generateResponse();
    send(client_socket, httpResponse.c_str(), httpResponse.size(), 0);
}
