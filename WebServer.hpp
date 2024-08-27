#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define TIME_OUT 60

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
#include "ServerBlock.hpp"
#include "Client.hpp"

std::string ft_itos(int nbr);

#endif
