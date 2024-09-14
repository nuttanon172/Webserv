#include "Request.hpp"

Request::Request(ServerConfig *serverBlock) : serverBlock(serverBlock)
{

}

Request::~Request()
{

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
	std::size_t pos;
	std::string fileName;
	std::getline(inputStream, buffer);
	std::stringstream stream(buffer);

	stream >> method;
	stream >> path;
	fileName = path;
	if (serverBlock->root[serverBlock->root.size() - 1] != '/' && path.size() != 1)
		path = serverBlock->root + fileName;
	else if (serverBlock->root[serverBlock->root.size() - 1] == '/' && path.size() > 1)
	{
		pos = path.find_first_not_of('/');
		fileName = path.substr(pos);
		path = serverBlock->root + fileName;
		//std::cout << "pos: " << pos << '\n';
	}
	else
		path = serverBlock->root;
	//std::cout << "Root: " << serverBlock->root << '\n';
	std::cout << "Path: " << path << '\n';
	return (true);
}

bool Request::parseHttpHeaders()
{
	std::string buffer = "";
	std::size_t colon;
	std::string key;
	std::string value;
	
	std::getline(inputStream, buffer);
	while (buffer.length() && buffer != "\r\n") // '\r\n' end line
	{
		colon = buffer.find_first_of(':');
		key = buffer.substr(0, colon);
		value = buffer.substr(colon + 2, buffer.size() - (colon + 2) - 1); // -1 trim \r
		//if (!key.length() || !value.length())
		//	return (false);
		header_map[key] = value;
		std::getline(inputStream, buffer);
	}
	return (true);
}

bool Request::parseBody()
{
	std::string buffer = "";
	body << inputStream.str();
	std::getline(inputStream, buffer);
	if (buffer != boundaryStart)
		return (false);
	while (std::getline(inputStream, buffer))
	{
		if (buffer == boundaryEnd)
			return (true);
	}
	return (false);
}

bool Request::isMultipart()
{
	if (header_map["Content-Type"].empty())
		return false;
	else if (header_map["Content-Type"].substr(0, 28) != "multipart/form-data; boundary")
		return false;
	size_t pos = header_map["Content-Type"].find("boundary=");
	if (pos != std::string::npos)
	{
		boundaryStart = header_map["Content-Type"].substr(pos + 9);
		boundaryEnd = "--" + boundaryEnd + "--";
	}
	return (true);
}

std::string &Request::getPath()
{
	return path;
}

std::string &Request::getMethod()
{
	return method;
}

std::map<std::string, std::string> &Request::getHeaderMap()
{
	return header_map;
}
