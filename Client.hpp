#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"

class Client
{
private:
	time_t _last_time;
	int socket;

public:
	void buildResponse();
	void updateTime();
	void clearClient();
	time_t getLastTime();
};

#endif
