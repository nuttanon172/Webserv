#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServer.hpp"

class Response
{
private:
	std::string _first_line;
	std::string _body;
	std::map<int, std::string> _status;
public:
	void buildFirstLine(int code);
	void buildBody();
	void buildIndex();
	void buildErrorBody();
	void buildResponse(int code);
	bool isDirectory(const std::string &path);
};

#endif