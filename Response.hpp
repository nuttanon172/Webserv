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
	std::string _message;
public:
	Response();
	Response(const Response &obj);
	Response &operator=(const Response &obj);
	~Response();
	void buildStatusLine(int code);
	void buildHeaders();
	void buildBody();
	void buildIndex();
	void buildErrorBody(int code);
	void buildHttpStatus(int code, int socket);
	void buildHttpMessages(int code);
	bool isDirectory(const std::string &path);
};

#endif