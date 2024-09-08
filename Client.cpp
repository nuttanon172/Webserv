#include "Client.hpp"

Client::Client(int socket, std::vector<ServerConfig> &paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request()), resp(new Response(paramBlocks)), serverBlock(paramBlocks)
{
}

Client::~Client()
{
	delete req;
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
	std::cout << "te\n";
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