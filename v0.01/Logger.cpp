#include "Logger.hpp"

// Log a generic message
void Logger::loginfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

// Log an error message
void Logger::errorinfo(const std::string& errorMessage) {
    std::cerr << "[ERROR] " << errorMessage << std::endl;
}

// Log a request with client IP and request details
void Logger::logRequestinfo(const std::string& clientIP, const std::string& requestDetails) {
    std::cout << "[REQUEST] From " << clientIP << ": " << requestDetails << std::endl;
}
