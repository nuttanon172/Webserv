#ifndef UTIL_HPP
#define UTIL_HPP

#include "config.hpp"

std::string ft_itos(int nbr);
void printConfig(const std::vector<ServerConfig> &servers);
std::map<int, std::string> initStatus();
std::string getCurrentTime();
bool isDirectory(const std::string &path);
bool isReadable(const std::string &path);
bool isExists(const std::string &path);
bool isNumber(std::string &str);
std::string filterSlashes(std::string path);
size_t ft_stost(std::string nbr);

#endif
