#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

class Logger {
public:
    // Log a generic message
    static void loginfo(const std::string& message);

    // Log an error message
    static void errorinfo(const std::string& errorMessage);

    // Log a request (could include details like method, resource, etc.)
    static void logRequestinfo(const std::string& clientIP, const std::string& requestDetails);
};

#endif // LOGGER_HPP
