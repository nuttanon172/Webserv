#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"

struct ServerConfig;
class Request;
class Response;

class Client
{
private:
	time_t last_time;
	int socket;
	Request *req;
	Response *resp;
	ServerConfig *serverBlock;

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
	std::string filterLocation();
};

#endif
