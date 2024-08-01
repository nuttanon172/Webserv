#include "Response.hpp"

void Response::buildResponse(int code)
{
	buildFirstLine(code);
}

void Response::buildFirstLine(int code)
{
	_first_line = "HTTP/1.1 " + ft_itos(code) + " " + _status[code];
}

void Response::buildBody()
{
}

void Response::buildIndex()
{
}

void Response::buildErrorBody()
{
}

bool isDirectory(const std::string &path) {}