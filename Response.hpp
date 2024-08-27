#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServer.hpp"

class Response
{
private:
	std::map<int, std::string> _status;
	std::string _first_line;
	std::string _header;
	std::string _body;
	std::map<int, std::string> _status;
public:
	void buildStatusLine(int code);
	void buildHeaders();
	void buildBody();
	void buildIndex();
	void buildErrorBody();
	void buildHttpStatus(int code);
	void buildHttpMessages(int code);
	bool isDirectory(const std::string &path);
};

#endif