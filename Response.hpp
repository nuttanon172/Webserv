#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServer.hpp"
#include "config.hpp"

class Request;

class Response
{
private:
	std::map<int, std::string> status;
	std::string status_line;
	std::string header;
	std::string body;
	std::string message;
	struct ServerConfig serverBlock;
public:
	Response(struct ServerConfig &serverBlock);
	Response(const Response &obj);
	Response &operator=(const Response &obj);
	~Response();
	bool searchFile(Request *req, int socket);
	void buildStatusLine(int code);
	void buildHeaders();
	void buildBody();
	void buildIndex();
	void buildErrorBody(int code);
	void buildHttpStatus(int code, int socket);
	void buildHttpMessages();
	void serveFile(std::string &path, int socket);
	void readFile(std::string &path, int socket);
	void sendFavicon(int socket);
	bool isDirectory(const std::string &path);
};

#endif