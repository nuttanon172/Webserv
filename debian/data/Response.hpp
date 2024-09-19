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
	ServerConfig *serverBlock;
public:
	Response(ServerConfig *serverBlock);
	Response(const Response &obj);
	Response &operator=(const Response &obj);
	~Response();
	void buildStatusLine(int code);
	void buildHeaders();
	void buildHeadersRedirect(std::string host, std::string &path);
	void buildBody(std::string body);
	void buildIndex();
	void buildErrorBody(int code);
	void buildHttpStatus(int code, int socket);
	void buildHttpMessages();
	bool searchFile(Request *req, int socket);
	void serveFile(std::string &path, int socket);
	void readFile(std::string &path, int socket);
	void redirectPath(Request *req, int code, int socket, std::string path);
	void serveCGI(std::string cgi_response, int socket);
	//void sendFavicon(int socket);
};

#endif
