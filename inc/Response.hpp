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
	std::string fileName;
	std::string content_type;
	ServerConfig *serverBlock;

public:
	Response(ServerConfig *serverBlock);
	Response(const Response &obj);
	Response &operator=(const Response &obj);
	~Response();
	void buildStatusLine(int code);
	void buildHeaders();
	void buildHeadersRedirect(std::string host, std::string &path);
	void buildIndex();
	void buildErrorBody(int code);
	void buildHttpCode(int code, int socket);
	void buildHttpMessages();
	bool searchFile(Request *req, int socket);
	void serveFile(std::string &path, std::string &reqPath, int socket);
	bool readFile(std::string &path, std::string &reqPath, int socket);
	void redirectPath(Request *req, int code, int socket, std::string path);
	void initContentType();
	bool isMethodAllow(std::string &method, std::string path);
	void serveCGI(std::string cgi_response, int socket);
};

#endif
