#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"

class Server{
private:
	int server_fd;
	int new_socket;
	int max_socket;
	struct sockaddr_in address;
	fd_set current_sockets;
	fd_set ready_sockets;
	//std::vector<ServerBlock> _server_vec;
	//std::map<int, ServerBlock> _server_map;
	std::map<int, std::string> _client_map;
public:
	Server();
	Server(const Server &obj);
	~Server();
	Server &operator=(const Server &obj);
	void initServer();
	void mainSever();
	void createSocket();
	void identifySocket();
	void checkClient();
	int recieveMessages();
	void closeSocket();
};

#endif
