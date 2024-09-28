#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define TIME_OUT 50000

#define BODY_SUCCESS 0
#define BODY_EXCEED 1
#define BODY_UNMATCH 2

#define DEFAULT "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RED "\033[31m"

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
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctime>
#include "Server.hpp"
#include "Response.hpp"
#include <sys/wait.h>
#include "Request.hpp"
#include "Client.hpp"
#include "util.hpp"
#include "CGI.hpp"
#include <dirent.h>
#include <sys/wait.h>

#endif
