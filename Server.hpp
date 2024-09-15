#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"

struct Location;
struct ServerConfig;
class Client;

class Server{
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
	bool readRequest(int socket);
	std::map<int, ServerConfig> server_config;
	std::map<int, Client *> client_map;
	//Request server_req;
public:
	//Server();
	//Server(const std::vector<ServerConfig> &obj);
	Server(const std::string &pathConfig);
	//Server(std::vector<ServerBlock> &serveBlock);
	~Server();
	//Server &operator=(const Server &obj);
	void initServer();
	void startServer();
	void initSocket();
	void identifySocket(int port, ServerConfig &serverBlock);
	void checkClient();
	void acceptNewConnection(int listen_sockets);
	void closeSocket(int socket);
	void shutdownServer();
};

#endif
