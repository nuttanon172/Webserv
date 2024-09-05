#ifndef ERRORHANDEL_HPP
#define ERRORHANDEL_HPP

#include <string>

class ErrorHandel {
private:
    std::string error_message;

public:
    // Constructor
    ErrorHandel();

    // Constructor with message
    ErrorHandel(const std::string& message);

    // Copy constructor
    ErrorHandel(const ErrorHandel& other);

    // Assignment operator
    ErrorHandel& operator=(const ErrorHandel& other);

    // Destructor
    ~ErrorHandel();

    // Setter
    void setErrorMessage(const std::string& message);

    // Getter
    std::string getErrorMessage() const;

    // Method to print error message
    void printError() const;
};

#endif // ERRORHANDEL_HPP
