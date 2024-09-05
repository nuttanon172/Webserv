#include "HttpResponse.hpp"

// Custom conversion function for C++98
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

HttpResponse::HttpResponse()
    : http_version("HTTP/1.1"), status_code(200), reason_phrase("OK") {}

HttpResponse::HttpResponse(int code, const std::string& reason)
    : http_version("HTTP/1.1"), status_code(code), reason_phrase(reason) {}

// Setters
void HttpResponse::setHttpVersion(const std::string& version) {
    http_version = version;
}

void HttpResponse::setStatusCode(int code) {
    status_code = code;
}

void HttpResponse::setReasonPhrase(const std::string& reason) {
    reason_phrase = reason;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void HttpResponse::setBody(const std::string& body_content) {
    body = body_content;
    // Automatically set Content-Length
    headers["Content-Length"] = intToString(body.size());
}

// Getters
std::string HttpResponse::getHttpVersion() const {
    return http_version;
}

int HttpResponse::getStatusCode() const {
    return status_code;
}

// std::string HttpResponse::getReasonPhrase() const {
//     return reason_phrase;
// }
std::string HttpResponse::getReasonPhrase(int status_code){

    status_code = status_code + 1;
    // std::map<int, std::string>::const_iterator it = status_reason_map.find(status_code);
    // if (it != status_reason_map.end()) {
    //     return it->second;
    // }
    return "Unknown Status";
}

std::map<std::string, std::string> HttpResponse::getHeaders() const {
    return headers;
}

std::string HttpResponse::getBody() const {
    return body;
}

// Generate the full HTTP response string
std::string HttpResponse::generateResponse() const {
    std::string response = http_version + " " + intToString(status_code) + " " + reason_phrase + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n" + body;
    return response;
}
