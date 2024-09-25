#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"
#define BUFFER_SIZE 8192 // 8KB buffer for reading data in chunks
#define BACKLOG 100
struct Location;
struct ServerConfig;
class Client;

class Server
{
private:
	int server_fd;
	int new_socket;
	int max_socket;
	struct sockaddr_in address;
	std::vector<ServerConfig> serverBlock;
	std::vector<int> socketVec;
	fd_set current_sockets;
	fd_set ready_sockets;
	fd_set listen_sockets;
	std::map<int, ServerConfig> server_config;
	std::map<int, Client *> client_map;

public:
	Server(const std::string &pathConfig);
	Server(const Server &obj);
	Server &operator=(const Server &obj);
	~Server();
	void initServer();
	void startServer();
	bool initSocket();
	bool identifySocket(int port, ServerConfig &serverBlock);
	void checkClient();
	void acceptNewConnection(int listen_sockets);
	void closeSocket(int socket);
	void shutdownServer();
	bool readRequest(int socket);
};

#endif
