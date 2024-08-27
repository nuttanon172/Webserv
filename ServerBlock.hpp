#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "WebServer.hpp"

class ServerBlock
{
public:
	std::vector<int> _portVec;
	std::string _severName;
	std::vector<std::string> _fullName;
	unsigned long _hostIP;
	std::string _root;
	size_t _clientMaxBodySize;
	std::vector<std::string> _indexVec;
	std::map<int, std::string> _errorMap;
	bool _autoIndex;
	std::size_t size;
	std::vector<int> &getPort();
};

#endif
