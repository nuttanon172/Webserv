#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServer.hpp"
#include "config.hpp"

class Request
{
private:
	std::stringstream inputStream;
	std::stringstream body;
	std::string method;
	std::string path;
	std::string boundaryStart;
	std::string boundaryEnd;
	ServerConfig *serverBlock;
	std::map<std::string, std::string> header_map;

public:
	Request(ServerConfig *serverBlock);
	~Request();
	std::string &getPath();
	std::string &getMethod();
	std::map<std::string, std::string> &getHeaderMap();
	void writeStream(char *str, int size);
	bool parseRequestLine();
	bool parseHttpHeaders();
	bool parseBody();
	bool isMultipart();
};

#endif
