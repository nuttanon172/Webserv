#ifndef UTIL_HPP
#define UTIL_HPP

#include "config.hpp"
#include "Logger.hpp"
#include <string>
#include <iostream>
#include <map>


void printServerStartMessage(const std::string& host, int port);

std::map<std::string, std::string> createMimeTypesMap();
// Declare mime_types as an external constant variable
extern const std::map<std::string, std::string> mime_types;

std::map<int, std::string>createStatusMap();

extern const std::map<int, std::string> status_reason_map ;

std::string getErrorPage(int error_code) ;

std::string getContentType(const std::string& uri) ;

// std::string getReasonPhrase(int status_code);

void printConfig(const std::vector<ServerConfig>& servers);



#endif // UTIL_HPP
