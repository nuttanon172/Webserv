#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"

class Client
{
private:
	time_t _last_time;
	int socket;
	Request req;
	std::vector<ServerBlock> serveBlock;

public:
	Request &getRequest();
	Client(int write_fd, int read_fd, std::vector<ServerBlock>  &serverBlocks);
	void buildResponse();
	void updateTime();
	void clearClient();
	time_t getLastTime();
};

#endif
