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
	Request *req;
	Response *resp;
	//std::vector<ServerConfig> serverBlock;
	ServerConfig *serverBlock;
	CGI *cgi;

public:
	Client(int socket, ServerConfig *paramBlocks);
	~Client();
	Request *getRequest();
	Response *getResponse();
	bool buildResponse();
	void updateTime();
	bool check_cgi();
	time_t getLastTime();
};

#endif
