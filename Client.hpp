#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"

struct ServerConfig;
class Request;
class Response;
class CGI;

class Client
{
private:
	time_t last_time;
	int socket;
	std::string serverName;
	Request *req;
	Response *resp;
	ServerConfig *serverBlock;
	CGI *cgi;

public:
	Client(int socket, ServerConfig *paramBlocks);
	Client(const Client &obj);
	Client &operator=(const Client &obj);
	~Client();
	Request *getRequest();
	Response *getResponse();
	bool buildResponse();
	void updateTime();
	time_t getLastTime();
	bool check_cgi();
	void setServerName(std::string serverName);
};

#endif
