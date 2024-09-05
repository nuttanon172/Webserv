#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>

class HttpResponse {
private:
    int status_code;
    std::string status_message;
    std::string body;
    std::map<int, std::string> error_pages;

public:
    // Constructor
    HttpResponse();

    // Copy constructor
    HttpResponse(const HttpResponse& other);

    // Assignment operator
    HttpResponse& operator=(const HttpResponse& other);

    // Destructor
    ~HttpResponse();

    // Setters
    void setStatusCode(int code);
    void setStatusMessage(const std::string& message);
    void setBody(const std::string& body_content);
    void setErrorPage(int code, const std::string& page_content);

    // Getters
    int getStatusCode() const;
    std::string getStatusMessage() const;
    std::string getBody() const;

    // Methods
    std::string generateResponse() const;
    std::string generateErrorResponse(int code) const;
};

// Custom conversion function for C++98
std::string intToString(int value);

#endif // HTTPRESPONSE_HPP
