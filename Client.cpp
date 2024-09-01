#include "Client.hpp"

Client::Client(int write_fd, int read_fd, std::vector<ServerBlock>  &serverBlocks)
{
	
}

void Client::buildResponse()
{

}

Request &Client::getRequest(){
	return req;
}

void Client::updateTime()
{
	_last_time = time(NULL);
}

void Client::clearClient()
{

}

time_t Client::getLastTime()
{
	return _last_time;
}