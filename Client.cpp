#include "Client.hpp"

Client::Client(int socket, ServerConfig *paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request(paramBlocks)), resp(new Response(paramBlocks)), serverBlock(paramBlocks)
{
}

Client::~Client()
{
	delete req;
	delete resp;
}

bool Client::buildResponse()
{
	std::stringstream ss;

	if (req->parseRequestLine() == false)
		return false;
	if (req->parseHttpHeaders() == false)
		return false;
	if (req->isMultipart() == true)
		req->parseBody();
	if (this->getResponse()->searchFile(this->getRequest(), socket) == true)
		this->getResponse()->serveFile(getRequest()->getPath(), getRequest()->getReqPath(), socket);
	return true;
}

Request *Client::getRequest()
{
	return req;
}

Response *Client::getResponse()
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