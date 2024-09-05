#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServer.hpp"

enum HttpStage;

class Request
{
private:
	std::stringstream inputStream;
	std::stringstream body;
	std::string method;
	std::string path;
	std::string boundaryStart;
	std::string boundaryEnd;
	std::map<std::string, std::string> header_map;
	HttpStage stage;

public:
	Request();
	~Request();
	void writeStream(char *str, int size);
	HttpStage parseRequestLine();
	HttpStage parseHttpHeaders();
	HttpStage parseBody();
	bool isMultipart();
};

#endif
