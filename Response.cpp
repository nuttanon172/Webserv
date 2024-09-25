#include "Response.hpp"

Response::Response(ServerConfig *serverBlock) : serverBlock(serverBlock)
{
	status = initStatus();
	status_line = "";
	header = "";
	body = "";
	message = "";
}

Response::Response(const Response &obj)
{
	*this = obj;
}

Response &Response::operator=(const Response &obj)
{
	if (this != &obj)
	{
		status = obj.status;
		status_line = obj.status_line;
		header = obj.header;
		body = obj.body;
		message = obj.message;
		serverBlock = obj.serverBlock;
	}
	return *this;
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
	ss << "<meta charset=\"UTF-8\">\n";
	ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	ss << "<title>" << code << " - " << status[code] << "</title>\n";
	ss << "<style>\n";
	ss << "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: Arial, sans-serif; background: #f4f4f4; color: #333; text-align: center; }\n";
	ss << ".container { background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); max-width: 500px; width: 90%; }\n";
	ss << "h1 { font-size: 3rem; margin: 0; }\n";
	ss << "p { font-size: 1rem; margin: 10px 0; }\n";
	ss << "a { color: #007bff; text-decoration: none; }\n";
	ss << "a:hover { text-decoration: underline; }\n";
	ss << "</style>\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << "<div class=\"container\">\n";
	ss << "<h1>" << code << "</h1>\n";
	ss << "<p>Error " << code << ": " << status[code] << "</p>\n";
	ss << "<p><a href=\"/\">Return to Homepage</a></p>\n";
	ss << "</div>\n";
	ss << "</body>\n";
	ss << "</html>\r\n";

	body = ss.str().c_str();
}

void Response::buildHttpCode(int code, int socket)
{
	buildStatusLine(code);
	buildErrorBody(code);
	buildHeaders();
	buildHttpMessages();
	send(socket, message.c_str(), message.size(), 0);
}

bool Response::isMethodAllow(std::string &method, std::string path)
{
	std::cout << "\tpath_allow: " << path << '\n';
	if (serverBlock->locations.empty() != true)
	{
		std::vector<Location>::iterator it_location = serverBlock->locations.begin();
		for (; it_location != serverBlock->locations.end(); it_location++)
		{
			if ((path == it_location->path || path == it_location->path + "/") && it_location->allow_methods.empty() != true)
			{
				std::map<std::string, bool>::iterator it_allow = it_location->allow_methods.begin();
				for (; it_allow != it_location->allow_methods.end(); it_allow++)
				{
					if (it_allow->first == method && it_allow->second == false)
						return false;
				}
			}
		}
	}
	return true;
}

void Response::readFile(std::string &path, std::string &reqPath, int socket)
{
	std::vector<Location>::iterator it_location;
	std::vector<std::string>::iterator it_server_index;
	std::vector<std::string>::iterator it_location_index;
	std::ifstream inputFile;
	std::string indexPath, tmp_path, location_path, line;
	std::string tmpIndex = "";
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

	std::cout << "\tpath: " << path << " " << path[path.size() - 1] << '\n';
	// check if it directory search for index inside else search for index
	if (path[path.size() - 1] != '/' && isReadable(path) == false)
	{
		this->buildErrorBody(403);
		return;
	}
	if (isDirectory(path) == true || access(path.c_str(), R_OK) != 0)
	{
		if (path[path.size() - 1] != '/' && isExists(path) == false)
		{
			tmp_path = filterSlashes(serverBlock->root + "/" + serverBlock->error_pages[404]);
			std::cout << "tmp_path: " << tmp_path << '\n';
			if (serverBlock->error_pages[404].empty() || access(tmp_path.c_str(), R_OK) != 0)
			{
				this->buildErrorBody(404);
				return;
			}
		}
		tmp_path = reqPath.substr(0, reqPath.find_last_of('/'));
		std::cout << "tmp_path: " << tmp_path << '\n';
		// search index of location
		if (location_index == true)
		{
			for (; it_location != serverBlock->locations.end(); it_location++)
			{
				std::cout << "location path: " << it_location->path << '\n';
				std::cout << "\ttmp_path: " << tmp_path << '\n';
				location_path = it_location->path;
				if (tmp_path == location_path || tmp_path == location_path + "/")
				{
					it_location_index = it_location->index.begin();
					for (; it_location_index != it_location->index.end(); it_location_index++)
					{
						indexPath = serverBlock->root + "/" + tmp_path + "/" + *it_location_index;
						indexPath = filterSlashes(indexPath);
						// std::cout << "Location FileName: " << *it_location_index << '\n';
						// std::cout << "Location FilePath: " << indexPath << '\n';
						if (isExists(indexPath))
						{
							if (access(indexPath.c_str(), R_OK) != 0 && tmpIndex.empty() == true)
								tmpIndex = indexPath;
							else
							{
								this->readFile(indexPath, reqPath, socket);
								return;
							}
						}
					}
					if (tmpIndex.empty() != false)
					{
						this->readFile(indexPath, reqPath, socket);
						return;
					}
				}
			}
		}
		std::cout << "tmp_path: " << tmp_path << '\n';
		// search index of server
		if (server_index == true)
		{
			for (; it_server_index != serverBlock->index.end(); it_server_index++)
			{
				indexPath = serverBlock->root + "/" + tmp_path + "/" + *it_server_index;
				indexPath = filterSlashes(indexPath);
				std::cout << "Server FileName: " << *it_server_index << '\n';
				std::cout << "Server FilePath: " << indexPath << '\n';
				if (isExists(indexPath))
				{
					if (access(indexPath.c_str(), R_OK) != 0 && tmpIndex.empty() == true)
						tmpIndex = indexPath;
					else
					{
						this->readFile(indexPath, reqPath, socket);
						return;
					}
				}
			}
			if (tmpIndex.empty() != false)
			{
				this->readFile(indexPath, reqPath, socket);
				return;
			}
		}
		else
			inputFile.open(tmp_path.c_str());
	}
	else
		inputFile.open(path.c_str());
	while (std::getline(inputFile, line))
		ss << line << "\r\n";
	body = ss.str();
}

bool Response::searchFile(Request *req, int socket)
{
	std::map<std::string, std::map<int, std::string> >::iterator it = serverBlock->location_return_path.begin();
	std::string req_path = req->getReqPath();

	std::cout << "\t\treq_path: " << req_path << '\n';
	if (req_path[req_path.size() - 1] != '/')
		return true;
	for (; it != serverBlock->location_return_path.end(); it++)
	{
		if (it->first == req_path || it->first + "/" == req_path)
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
	readFile(path, reqPath, socket);
	buildHeaders();
	buildHttpMessages();
	std::cout << "serveFile called\n";
	send(socket, message.c_str(), message.size(), 0);
}

void Response::buildBody()
{
}

void Response::serveCGI(std::string cgi_response, int socket)
{
	buildStatusLine(200);
	this->body = cgi_response;
	if (this->body.size() < 34 || this->body.substr(0, 34) != "Content-Description: File Transfer")
		buildHeaders();
	// buildHeaders();
	buildHttpMessages();
	std::cout << "----------------\n"
			  << message.c_str() << std::endl;
	send(socket, message.c_str(), message.size(), 0);
}
