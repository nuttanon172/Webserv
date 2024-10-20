#ifndef UTIL_HPP
#define UTIL_HPP

#include "config.hpp"

std::string ft_itos(int nbr);
std::string getCurrentTime();
std::string filterSlashes(std::string path);
std::string List_file(std::string path, std::string reqPath);
bool isDirectory(const std::string &path);
bool isReadable(const std::string &path);
bool isExists(const std::string &path);
bool isNumber(std::string &str);
std::map<int, std::string> initStatus();
size_t ft_stost(std::string nbr);
void printConfig(const std::vector<ServerConfig> &servers);

#endif
