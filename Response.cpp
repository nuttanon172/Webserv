#include "Response.hpp"

void Response::buildHttpMessages(int code)
{
	buildStatusLine(code);
	buildHeaders();
}

void Response::buildStatusLine(int code)
{
	_first_line = "HTTP/1.1 " + ft_itos(code) + " " + _status[code];
}

void Response::buildHeaders()
{
	std::cout << "Host: " << "\n";
	std::cout << "Connection: Keep-Alive\n";
	std::cout << "Keep-Alive: timeout=6, max=20\n";
	std::cout << "Content-Type: " << "\n";
	std::cout << "Content-Length: " << "\n";
}

void Response::buildHttpStatus(int code)
{
	buildStatusLine(code);
	buildHeaders();
	std::cout << "<!DOCTYPE html>\n";
	std::cout << "<html>\n";
	std::cout << "<head>\n";
	std::cout << "<title>" << code << "-" << _status[code] << "</title>\n";
	std::cout << "</head>\n";
	std::cout << "<body>\n";
	if (code < 200 || code > 299)
		std::cout << "<h1>" << code << "</h1>\n";
	else
		std::cout << "<h1>Error " << code << "</h1>\n";
	std::cout << _status[code];
	std::cout << "</body>\n";
	std::cout << "</html>\n";
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