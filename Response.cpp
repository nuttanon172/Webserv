#include "Response.hpp"

Response::Response(ServerConfig *serverBlock) : serverBlock(serverBlock)
{
	status = initStatus();
	status_line = "";
	header = "";
	body = "";
	message = "";
}

Response::~Response()
{
	status_line = "";
	header = "";
	body = "";
	message = "";
}

void Response::buildHttpMessages()
{
	message = status_line + header + body;
}

void Response::buildStatusLine(int code)
{
	status_line = "HTTP/1.1 " + ft_itos(code) + " " + status[code] + "\r\n";
}

void Response::buildHeaders()
{
	std::stringstream ss;

	ss << "Date: " + getCurrentTime() + "\r\n";
	if (serverBlock->server_name.empty() != true)
		ss << "Server: " << serverBlock->server_name << "\r\n";
	ss << "Connection: Keep-Alive\r\n";
	// ss << "Keep-Alive: timeout=6, max=2\r\n"; no directive require
	ss << "Content-Type: text/html; charset=UTF-8" << "\r\n";
	ss << "Content-Length: " << body.size() << "\r\n";
	ss << "\r\n";
	header = ss.str().c_str();
}

void Response::buildHeadersRedirect(std::string host, std::string &path)
{
	std::stringstream ss;

	if (host[0] != '/')
		host = "/" + host;
	if (serverBlock->server_name.empty() != true)
	{
		ss << "Server: " << serverBlock->server_name << "\r\n";
		ss << "Location: " << "http://" << host << path << "\r\n";
	}
	else
		ss << "Location: " << "http://" << host << path << "\r\n";
	ss << "Date: " + getCurrentTime() + "\r\n";
	ss << "Connection: Keep-Alive\r\n";
	ss << "Content-Type: text/html; charset=UTF-8" << "\r\n";
	ss << "Content-Length: 0" << "\r\n";
	ss << "\r\n";
	header = ss.str().c_str();
}

void Response::buildErrorBody(int code)
{
	std::stringstream ss;

	ss << "<!DOCTYPE html>\n";
	ss << "<html lang=\"en\">\n";
	ss << "<head>\n";
	ss << "<title>" << code << "-" << status[code] << "</title>\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << "<h1>" << code << "</h1>\n";
	ss << "<h1>Error " << code << "</h1>\n";
	ss << "<p>" << status[code] << ".</p>" << '\n';
	ss << "</body>\n";
	ss << "</html>\r\n";
	body = ss.str().c_str();
}

void Response::buildHttpStatus(int code, int socket)
{
	buildStatusLine(code);
	buildErrorBody(code);
	buildHeaders();
	buildHttpMessages();
	send(socket, message.c_str(), message.size(), 0);
}

void Response::readFile(std::string &path, std::string &reqPath, int socket)
{
	std::vector<Location>::iterator it_location;
	std::vector<std::string>::iterator it_server_index;
	std::vector<std::string>::iterator it_location_index;
	std::string indexPath, tmp_path, location_path, line;
	std::ifstream inputFile(path.c_str());
	std::stringstream ss;
	bool location_index = true;
	bool server_index = true; 

	if (serverBlock->locations.empty() != true)
		it_location = serverBlock->locations.begin();
	else
		location_index = false;
	if (serverBlock->index.empty() != true)
		it_server_index = serverBlock->index.begin();
	else
		server_index = false;
	if (location_index == true)
		it_location_index = it_location->index.begin();

	// check if it directory search for index inside else search for index
	if (isDirectory(path) == true || access(path.c_str(), R_OK) != 0)
	{
		if (path[path.size() - 1] != '/')
			tmp_path = path + "/";
		else
			tmp_path = path;
		//std::cout << "tmp_path: " << tmp_path << '\n';
		if (location_index == true)
		{
			for (; it_location != serverBlock->locations.end(); it_location++)
			{
				//std::cout << "location path: " << it_location->path << '\n';
				//std::cout << "reqPath: " << reqPath << '\n';
				location_path = it_location->path;
				if (reqPath == location_path || reqPath == location_path + "/")
				{
					it_location_index = it_location->index.begin();
					for (; it_location_index != it_location->index.end(); it_location_index++)
					{
						indexPath = tmp_path + *it_location_index;
						//std::cout << "Location FileName: " << *it_location_index << '\n';
						//std::cout << "Location FilePath: " << indexPath << '\n';
						if (access(indexPath.c_str(), R_OK) == 0)
						{
							path = indexPath;
							this->readFile(path, reqPath, socket);
							return;
						}
					}
				}
			}
		}
		//std::cout << "tmp_path: " << tmp_path << '\n';
		if (server_index == true)
		{
			for (; it_server_index != serverBlock->index.end(); it_server_index++)
			{
				indexPath = tmp_path + *it_server_index;
				//std::cout << "Server FileName: " << *it_server_index << '\n';
				//std::cout << "Server FilePath: " << indexPath << '\n';
				if (access(indexPath.c_str(), R_OK) == 0)
				{
					path = indexPath;
					this->readFile(path, reqPath, socket);
					return;
				}
			}
		}
		tmp_path = tmp_path + serverBlock->error_pages[404];
		//std::cout << "tmp_path: " << tmp_path << '\n';
		if (serverBlock->error_pages[404].empty() || access(tmp_path.c_str(), R_OK) != 0)
			this->buildHttpStatus(404, socket);
	}
	while (std::getline(inputFile, line))
		ss << line << "\r\n";
	body = ss.str();
}

bool Response::searchFile(Request *req, int socket)
{
	std::map<std::string, std::map<int, std::string> >::iterator it = serverBlock->location_return_path.begin();
	std::string req_path = req->getReqPath();

	for (; it != serverBlock->location_return_path.end(); it++)
	{
		if (req_path == it->first)
		{
			std::map<int, std::string>::iterator it_return = it->second.begin();
			this->redirectPath(req, it_return->first, socket, it_return->second);
			return false;
		}
	}
	return true;
}

void Response::redirectPath(Request *req, int code, int socket, std::string path)
{
	buildStatusLine(code);
	buildHeadersRedirect(req->getHost(), path);
	buildHttpMessages();
	std::cout << message;
	send(socket, message.c_str(), message.size(), 0);
}

void Response::serveFile(std::string &path, std::string &reqPath, int socket)
{
	buildStatusLine(200);
	// path = "/home/ntairatt/WebServ/docs/fusion_web/index.html";
	readFile(path, reqPath, socket);
	buildHeaders();
	buildHttpMessages();
	std::cout << "serveFile called\n";
	send(socket, message.c_str(), message.size(), 0);
}

void Response::buildBody()
{
}
