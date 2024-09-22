#include "Client.hpp"

Client::Client(int socket, ServerConfig *paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request(paramBlocks)), resp(new Response(paramBlocks)), serverBlock(paramBlocks)
{
}

Client::Client(const Client &obj)
{
	*this = obj;
}

Client &Client::operator=(const Client &obj)
{
	if (this != &obj)
	{
		last_time = obj.last_time;
		socket = obj.socket;
		req = new Request(obj.serverBlock);
		resp = new Response(obj.serverBlock);
		serverBlock = obj.serverBlock;
	}
	return *this;
}

Client::~Client()
{
	delete req;
	delete resp;
}

bool Client::buildResponse()
{
	std::stringstream ss;
	(void)serverBlock;

	if (req->parseRequestLine() == false)
	{
		resp->buildHttpCode(400, socket);
		return true;
	}
	if (req->parseHttpHeaders() == false)
	{
		resp->buildHttpCode(400, socket);
		return true;
	}
	if (req->isMultipart() == true)
		req->parseBody();
	if (resp->isMethodAllow(req->getMethod(), this->filterLocation()) == false)
	{
		resp->buildHttpCode(405, socket);
		return true;
	}
	if (this->getResponse()->searchFile(this->getRequest(), socket) == true)
		this->getResponse()->serveFile(getRequest()->getPath(), getRequest()->getReqPath(), socket);
	return true;
}

std::string Client::filterLocation()
{
	std::string path = req->getReqPath();
	path = path.substr(0, path.find_last_of('/')) + "/";
	return (path);
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