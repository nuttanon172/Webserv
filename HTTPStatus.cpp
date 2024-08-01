#include "HTTPStatus.hpp"

std::map<int, std::string> initStatus()
{
	std::map<int, std::string> status;

	status[200] = "OK";
	status[201] = "Created";
	status[301] = "Moved Permanently";
	status[307] = "Temporary Redirect";
	status[400] = "Bad Request";
	status[403] = "ForBidden";
	status[404] = "Not Found";
	status[405] = "Method Not Allowed";
	status[413] = "Content Too Large";
	status[422] = "Unprocessable Content";
	status[500] = "Internal Server Error";
	status[502] = "Bad Gateway";
	return status;
}