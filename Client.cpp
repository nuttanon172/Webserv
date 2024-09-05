#include "Client.hpp"

Client::Client(int socket, std::vector<ServerConfig> &paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request()), serverBlock(paramBlocks)
{
}

Client::~Client()
{
	delete req;
}

void Client::buildResponse()
{
	req->parseRequestLine();
	req->parseHttpHeaders();
	if (req->isMultipart() == true)
		req->parseBody();
}

Request *Client::getRequest()
{
	return req;
}

void Client::updateTime()
{
	last_time = time(NULL);
}

time_t Client::getLastTime()
{
	return last_time;
}