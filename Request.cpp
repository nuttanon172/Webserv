#include "Request.hpp"

void Request::writeStream(char *str, int size)
{
	if (size < 0)
		return;
	inputStream.write(str, size);
}

HttpStage Request::parseRequestLine(HttpStage stage)
{
	std::string buffer = "";
	std::getline(inputStream, buffer);
	std::stringstream stream(buffer);

	stream >> method;
	stream >> path;
	stage = HEADER;
}

HttpStage Request::parseHttpHeaders(HttpStage stage)
{
	std::string buffer = "";
	std::size_t colon;
	std::string key;
	std::string value;
	
	std::getline(inputStream, buffer);
	while (buffer.length() && buffer != "\r\n") // '\r\n' end of header
	{
		colon = buffer.find_first_of(':');
		if (colon == std::string::npos)
		{
			stage = ERROR;
			return (stage);
		}
		key = buffer.substr(0, colon);
		value = buffer.substr(colon + 2, buffer.size() - (colon + 2) - 1); // -1 trim \r
		if (!key.length() || !value.length())
		{
			stage = ERROR;
			return (stage);
		}
		header_map[key] = value;
		std::getline(inputStream, buffer);
	}
}

HttpStage Request::parseBody(HttpStage stage)
{
	std::string buffer = "";
	std::stringstream ss;
	ss << inputStream.rdbuf();
	std::getline(ss, buffer);
	if (buffer != boundaryStart)
		stage = ERROR;
	while (std::getline(ss, buffer))
	{
		if (buffer == boundaryEnd)
			return (ROUTER);
	}
}

bool Request::isMultipart()
{
	if (header_map["Content-Type"].empty())
	{
		stage = ERROR;
	}
	else if (header_map["Content-Type"].substr(0, 28) != "multipart/form-data; boundary")
	{
		stage = ERROR;
	}
	size_t pos = header_map["Content-Type"].find("boundary=");
	if (pos != std::string::npos)
	{
		boundaryStart = header_map["Content-Type"].substr(pos + 9);
		boundaryEnd = "--" + boundaryEnd + "--";
	}
	return (true);
}