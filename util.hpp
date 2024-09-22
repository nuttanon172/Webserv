#ifndef UTIL_HPP
#define UTIL_HPP

#include "config.hpp"

std::string ft_itos(int nbr);
void printConfig(const std::vector<ServerConfig> &servers);
std::map<int, std::string> initStatus();
std::string getCurrentTime();
bool isDirectory(const std::string &path);
std::string filterSlashes(std::string path);
bool isNumber(std::string &str);
void printProcessingStatus();

#endif
