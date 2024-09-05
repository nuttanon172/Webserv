#include "ErrorHandel.hpp"
#include <iostream>

// Default Constructor
ErrorHandel::ErrorHandel() : error_message("Unknown Error") {}

// Constructor with message
ErrorHandel::ErrorHandel(const std::string& message) : error_message(message) {}

// Copy Constructor
ErrorHandel::ErrorHandel(const ErrorHandel& other) : error_message(other.error_message) {}

// Assignment Operator
ErrorHandel& ErrorHandel::operator=(const ErrorHandel& other) {
    if (this != &other) {
        error_message = other.error_message;
    }
    return *this;
}

// Destructor
ErrorHandel::~ErrorHandel() {}

// Setter
void ErrorHandel::setErrorMessage(const std::string& message) {
    error_message = message;
}

// Getter
std::string ErrorHandel::getErrorMessage() const {
    return error_message;
}

// Method to print error message
void ErrorHandel::printError() const {
    std::cerr << "Error: " << error_message << std::endl;
}
