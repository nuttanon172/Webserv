#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP

#include "WebServer.hpp"

enum HttpStage {
	FIRST_LINE,
	HEADER,
	BODY,
	RESPONSED,
	ROUTER,
	ERROR,
};

std::map<int, std::string> initStatus();

#endif
