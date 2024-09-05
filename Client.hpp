#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"

struct ServerConfig;
class Request;

class Client
{
private:
	time_t last_time;
	int socket;
	Request *req;
	std::vector<ServerConfig> serverBlock;

public:
	Request *getRequest();
	Client(int socket, std::vector<ServerConfig> &paramBlocks);
	~Client();
	void buildResponse();
	void updateTime();
	time_t getLastTime();
};

#endif
