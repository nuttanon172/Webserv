#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>
#include <sstream>

class HttpResponse {
private:
    std::string http_version;
    int status_code;
    std::string reason_phrase;
    std::map<std::string, std::string> headers;
    std::string body;

public:
    // Constructors
    HttpResponse();
    HttpResponse(int code, const std::string& reason);

    // Setters
    void setHttpVersion(const std::string& version);
    void setStatusCode(int code);
    void setReasonPhrase(const std::string& reason);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body_content);

    // Getters
    std::string getHttpVersion() const;
    int getStatusCode() const;
    std::string getReasonPhrase(int status_code);
    std::map<std::string, std::string> getHeaders() const;
    std::string getBody() const;

    // Generate the full HTTP response string
    std::string generateResponse() const;
    // Custom conversion function for C++98
  
};
  std::string intToString(int value);

#endif // HTTPRESPONSE_HPP
