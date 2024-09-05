#include "HttpResponse.hpp"
#include <sstream>

// Constructor
HttpResponse::HttpResponse() : status_code(200), status_message("OK"), body("") {}

// Copy Constructor
HttpResponse::HttpResponse(const HttpResponse& other)
    : status_code(other.status_code),
      status_message(other.status_message),
      body(other.body),
      error_pages(other.error_pages) {}

// Assignment Operator
HttpResponse& HttpResponse::operator=(const HttpResponse& other) {
    if (this != &other) {
        status_code = other.status_code;
        status_message = other.status_message;
        body = other.body;
        error_pages = other.error_pages;
    }
    return *this;
}

// Destructor
HttpResponse::~HttpResponse() {}

// Setters
void HttpResponse::setStatusCode(int code) { status_code = code; }
void HttpResponse::setStatusMessage(const std::string& message) { status_message = message; }
void HttpResponse::setBody(const std::string& body_content) { body = body_content; }
void HttpResponse::setErrorPage(int code, const std::string& page_content) {
    error_pages[code] = page_content;
}

// Getters
int HttpResponse::getStatusCode() const { return status_code; }
std::string HttpResponse::getStatusMessage() const { return status_message; }
std::string HttpResponse::getBody() const { return body; }

// Methods
std::string HttpResponse::generateResponse() const {
    std::string response = "HTTP/1.1 " + intToString(status_code) + " " + status_message + "\r\n";
    response += "Content-Length: " + intToString(body.size()) + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n" + body;
    return response;
}

std::string HttpResponse::generateErrorResponse(int code) const {
    std::map<int, std::string>::const_iterator it = error_pages.find(code);
    if (it != error_pages.end()) {
        return it->second;
    }
    return "<html><body><h1>Error " + intToString(code) + "</h1></body></html>";
}

// Custom conversion function for C++98
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
