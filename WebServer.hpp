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
#include "Server.hpp"
//#include "Client.hpp"
#include "HTTPStatus.hpp"
#include "config.hpp"
//#include "Request.hpp"
#include "util.hpp"

std::string ft_itos(int nbr);

#endif
