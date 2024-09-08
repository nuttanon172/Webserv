#include "Response.hpp"

Response::Response()
{
	_status = initStatus();
	_status_line = "";
	_header = "";
	_body = "";
	_message = "";
}

Response::~Response()
{
	_status_line = "";
	_header = "";
	_body = "";
	_message = "";
}

void Response::buildHttpMessages()
{
	_message = _status_line + _header + _body;
}

void Response::buildStatusLine(int code)
{
	_status_line = "HTTP/1.1 " + ft_itos(code) + " " + _status[code];
}

void Response::buildHeaders()
{
	std::stringstream ss;

	ss << "\r\nHost: localhost:8002" << "\r\n";
	ss << "Connection: Keep-Alive\r\n";
	ss << "Keep-Alive: timeout=6, max=2\r\n";
	ss << "Content-Type: text/html" << "\r\n\r\n";
	//ss << "Content-Length: " << "\r\n\r\n";
	_header = ss.str().c_str();
}

void Response::buildErrorBody(int code)
{
	std::stringstream ss;

	ss << "<!DOCTYPE html>\n";
	ss << "<html lang=\"en\">\n";
	ss << "<head>\n";
	ss << "<title>" << code << "-" << _status[code] << "</title>\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << "<h1>" << code << "</h1>\n";
	ss << "<h1>Error " << code << "</h1>\n";
	ss << "<p>" << _status[code] << ".</p>"<< '\n';
	ss << "</body>\n";
	ss << "</html>\r\n";
	_body = ss.str().c_str();
}

void Response::buildHttpStatus(int code, int socket)
{
	buildStatusLine(code);
	buildHeaders();
	buildErrorBody(code);
	buildHttpMessages();
	send(socket, _message.c_str(), _message.size(), 0);
}

void Response::readFile(std::string &path, int socket)
{
	std::ifstream inputFile(path.c_str());
	std::string line;
	std::stringstream ss;

	if (access(path.c_str(), R_OK) != 0)
	{
		std::cerr << "readFile: Read file error\n";
		buildHttpStatus(403, socket);
	}
	while (std::getline(inputFile, line))
	{
		ss << line << "<br>";
		ss << '\n';
	}
	_body = ss.str();
}

void Response::serveFile(std::string &path, int socket)
{
	buildStatusLine(200);
	buildHeaders();
	readFile(path, socket);
	buildHttpMessages();
	std::cout << _status_line;
	std::cout << _header;
	std::cout << _body;
	send(socket, _message.c_str(), _message.size(), 0);
}

void Response::buildBody()
{

}

//bool isDirectory(const std::string &path) {}