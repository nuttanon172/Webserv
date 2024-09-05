#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServer.hpp"

class Request
{
	private:
		std::stringstream inputStream;
		std::string method;
		std::string path;
		std::string body;
		std::string boundaryStart;
		std::string boundaryEnd;
		std::map<std::string, std::string> header_map;
		HttpStage stage;

	public:
		void writeStream(char *str, int size);
		HttpStage parseRequestLine(HttpStage stage);
		HttpStage parseHttpHeaders(HttpStage stage);
		HttpStage parseBody(HttpStage stage);
		bool isMultipart();
};

#endif
