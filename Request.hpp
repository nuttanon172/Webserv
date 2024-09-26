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
	std::string req_path;
	std::string protocal;
	std::string boundaryStart;
	std::string boundaryEnd;
	ServerConfig *serverBlock;
	std::map<std::string, std::string> header_map;
	std::map<std::string, std::string> cgi_map;

public:
	Request(ServerConfig *serverBlock);
	Request(const Request &obj);
	Request &operator=(const Request &obj);
	~Request();
	std::string &getPath();
	std::string &getMethod();
	std::string &getReqPath();
	std::string &getHost();
	std::stringstream &getBody();
	std::map<std::string, std::string> &getHeaderMap();
	void writeStream(char *str, int size);
	std::string makePath(std::string &dest);
	bool parseRequestLine();
	bool parseHttpHeaders(std::string &serverName);
	bool parseBody();
	bool isMultipart();
	bool isCorrectHost();
};

#endif
