#include "Request.hpp"

Request::Request(ServerConfig *serverBlock) : serverBlock(serverBlock)
{

}

Request::Request(const Request &obj)
{
	*this = obj;
}

Request &Request::operator=(const Request &obj)
{
	if (this != &obj)
	{
		inputStream << obj.inputStream.rdbuf();
		body << obj.body.rdbuf();
		method = obj.method;
		path = obj.method;
		req_path = obj.method;
		boundaryStart = obj.boundaryStart;
		boundaryEnd = obj.boundaryEnd;
		serverBlock = obj.serverBlock;
		header_map = obj.header_map;
	}
	return *this;
}

Request::~Request()
{
	inputStream.clear();
	body.clear();
}

void Request::writeStream(char *str, int size)
{
	if (size < 0)
		return;
	inputStream.write(str, size);
}

bool Request::parseRequestLine()
{
	std::string buffer = "";
	std::getline(inputStream, buffer);
	std::stringstream stream(buffer);

	stream >> method;
	stream >> path;
	stream >> protocal;
	path = filterSlashes(path);
	if (path[0] != '/' || path.find_first_of('\\') != std::string::npos)
		return false;
	if (method != "GET" && method != "POST" && method != "DELETE")
		return false;
	if (protocal != "HTTP/1.1")
		return false;
	req_path = path;
	path = makePath(path); // return full path
	// std::cout << "Root: " << serverBlock->root << '\n';
	std::cout << "Request Full Path: " << path << '\n';
	std::cout << "Request Path: " << req_path << '\n';
	return true;
}

std::string Request::makePath(std::string &dest)
{
	std::size_t pos;
	std::string fileName;

	fileName = dest;
	if (serverBlock->root[serverBlock->root.size() - 1] != '/' && dest.size() != 1)
		return (serverBlock->root + fileName);
	else if (serverBlock->root[serverBlock->root.size() - 1] == '/' && dest.size() > 1)
	{
		pos = dest.find_first_not_of('/');
		fileName = dest.substr(pos);
		return (serverBlock->root + fileName);
		// std::cout << "pos: " << pos << '\n';
	}
	else
		return (serverBlock->root);
}

bool Request::parseHttpHeaders(std::string &serverName)
{
	std::string buffer = "";
	std::size_t colon;
	std::string key;
	std::string value;

	std::getline(inputStream, buffer);
	while (buffer.length() && buffer != "\r")
	{
		colon = buffer.find_first_of(':');
		key = buffer.substr(0, colon);
		value = buffer.substr(colon + 2, (buffer.size() - (colon + 2) - 1)); // remove \r
		if ((key == "Content-Length") && (isNumber(value) == false))
			return false;
		header_map[key] = value;
		buffer.clear();
		std::getline(inputStream, buffer);
	}
	if (header_map["Host"].empty() == true || header_map["Host"] != serverName)
		return false;
	return true;
}

int Request::parseBody()
{
	std::string buffer;
	std::string key;
	std::string value;
	std::stringstream ss;
	int count_loop;
	size_t i;

	i = 0;
	count_loop = 0;
	while (std::getline(inputStream, buffer))
	{
		count_loop++;
		i += (buffer.length() + 1);
		body << buffer << "\n";
		buffer.clear();
	}
	if (count_loop == 1)
		i--;
	// std::cout << "parsebody: \n" << body.str() << std::endl;
	std::cout << "content-Lenght: " << i <<std::endl;
	if (ft_stost(header_map["Content-Length"]) != i)
		return (BODY_UNMATCH);
	std::vector<Location>::iterator it_location = serverBlock->locations.begin();
	for (;it_location != serverBlock->locations.end();it_location++)
	{
		if (getReqPath() == it_location->path || getReqPath() == it_location->path + "/")
		{
			if (!it_location->client_max_body_size)
				return (BODY_SUCCESS);
			else if (it_location->client_max_body_size  && it_location->client_max_body_size >= i)
				return (BODY_SUCCESS);
		}
	}
	if (!serverBlock->client_max_body_size)
		return (BODY_SUCCESS);
	else if (serverBlock->client_max_body_size && serverBlock->client_max_body_size >= i)
		return (BODY_SUCCESS);
	return (BODY_EXCEED);
}

bool Request::isMultipart()
{
	if (header_map["Content-Type"].empty())
		return false;
	else if (header_map["Content-Type"].substr(0, 29) != "multipart/form-data; boundary")
		return false;
	size_t pos = header_map["Content-Type"].find("boundary=");
	if (pos != std::string::npos)
	{
		boundaryStart = "--" + header_map["Content-Type"].substr(pos + 9);
		boundaryEnd = "--" + header_map["Content-Type"].substr(pos + 9) + "--";
	}
	return (true);
}

std::string &Request::getPath()
{ 
	return path;
}

std::string &Request::getReqPath()
{
	return req_path;
}

std::string &Request::getMethod()
{
	return method;
}

std::map<std::string, std::string> &Request::getHeaderMap()
{
	return header_map;
}

std::string &Request::getHost()
{
	return header_map["Host"];
}

std::stringstream &Request::getBody()
{
	return body;
}
