#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define TIME_OUT 6

#define DEFAULT "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RED     "\033[31m"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctime>
#include "Server.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include "config.hpp"
#include "util.hpp"

std::string ft_itos(int nbr);

#endif
