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

	public:
		void writeStream(char *str, int size);
		HttpStage parseRequestLine(HttpStage stage, int socket);
		HttpStage parseHttpHeaders(HttpStage stage, int socket);
		HttpStage parseBody(HttpStage stage, int socket);
};

#endif
