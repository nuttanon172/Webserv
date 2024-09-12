#include "Client.hpp"

Client::Client(int socket, ServerConfig *paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request()), resp(new Response(paramBlocks[0])), serverBlock(paramBlocks)
{
}

Client::~Client()
{
	delete req;
	delete resp;
}

void Client::buildResponse()
{
	std::stringstream ss;

	if (req->parseRequestLine() == false)
		return ;
	if (req->parseHttpHeaders() == false)
		return ;
	if (req->isMultipart() == true)
		req->parseBody();
	this->updateTime();
	this->getResponse()->serveFile(getRequest()->getPath(), socket);
}

Request *Client::getRequest() const
{
	return req;
}

Response *Client::getResponse() const
{
	return resp;
}

void Client::updateTime()
{
	last_time = time(NULL);
}

time_t Client::getLastTime()
{
	return last_time;
}