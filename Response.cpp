#include "Response.hpp"

Response::Response()
{
	_status = initStatus();
	_first_line = "";
	_header = "";
	_body = "";
	_message = "";
}

Response::~Response()
{
	_first_line = "";
	_header = "";
	_body = "";
	_message = "";
}

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
	std::stringstream ss;

	ss << "Host: localhost:" << "\r\n";
	ss << "Connection: Keep-Alive\r\n";
	ss << "Keep-Alive: timeout=6, max=2\r\n";
	ss << "Content-Type: " << "\r\n";
	ss << "Content-Length: " << "\r\n";
	_header = ss.str().c_str();
}

void Response::buildErrorBody(int code)
{
	std::stringstream ss;

	ss << "<!DOCTYPE html>\n";
	ss << "<html>\n";
	ss << "<head>\n";
	ss << "<title>" << code << "-" << _status[code] << "</title>\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << "<h1>" << code << "</h1>\n";
	ss << "<h1>Error " << code << "</h1>\n";
	ss << _status[code];
	ss << "</body>\n";
	ss << "</html>\n";
	_body = ss.str().c_str();
}

void Response::buildHttpStatus(int code, int socket)
{
	buildStatusLine(code);
	buildHeaders();
	buildErrorBody(code);
	std::string _message = _first_line + _header + _body;
	send(socket, _message.c_str(), _message.size(), 0);
}

void Response::buildBody()
{
}

void Response::buildIndex()
{
}

//bool isDirectory(const std::string &path) {}