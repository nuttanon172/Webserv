#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include <map>
#include <vector>
#include <iostream>
#include "LocationBlock.hpp"
#include "utils.hpp"
#include <arpa/inet.h>

class LocationBlock;

class ServerBlock {
	
	public:
		ServerBlock();
		ServerBlock(std::string const &serverBlock);
		ServerBlock(ServerBlock const &serverBlockInstance);
		ServerBlock &operator= (ServerBlock const &serverBlockInstance);
		~ServerBlock();

		std::vector<size_t>			getPortNumb();
		// size_t						getPortNumb();
		unsigned long				getHostIP();
		std::string					getServerName();
		std::string					getRoot();
		size_t						getClientMaxBodySize(); //P'karn
		std::vector<std::string>	getIndex();
		bool						getAutoindex();
		std::map<int, std::string>	getErrorPage();
		std::vector<LocationBlock>&	getLocationBlocks();
		std::vector<int>			getSocket();
		std::vector<std::string>	getFullName();
		std::vector<std::string>	getRawPort();
		std::string					getRawHostIP();
		// std::map<std::string , LocationBlocks> getLocationMap();
		

		void						setPortNumb(size_t val);
		void						setHostIP(unsigned long val);
		void						setBindingPort(std::string val1, std::vector<std::string> val2);
		void						setServerName(std::string val);
		void						setRawPort(std::string val);
		void						setRoot(std::string val);
		void						setClientMaxBodySize(size_t val);
		void						setIndex(std::vector<std::string> val);
		void						setAutoindex(bool val);
		void						setErrorPage(int key, std::string val);
		void						setLocationMap(std::string directoryPath, LocationBlock const &locationBlock);

		bool						manageSocket();
		std::string					pathToErrorPage(std::string values);
		void						DebugServerBlock();
		void						validateRoot(std::string root);   
		void						validateFullHost(std::string &hostname);
		void						is_portduplicate(size_t port);
		void						checkAllparametersAfterParsing();


	//private:
		void						__initServer(std::string const &serverBlock);
		void						__initServerParameters(std::string const &directive, std::vector<std::string> values);

		std::vector<int>			_socket_fd;
		std::vector<std::string>	_rawPort;
		int				   			_sock;
		std::string					_rawHostIP;
		std::vector<size_t>			_portNumb;
		unsigned long				_hostIP; //setup host IP address
		size_t						_clientMaxBodySize;
		std::string					_serverNames;
		std::vector<std::string>	_bindingPort;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<LocationBlock>	_locationBlocks;
		std::map<int, std::string>	_errorPage;
		bool						_autoIndex;

		std::map<std::string, LocationBlock>	_locationMap;
};

#endif