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
	fd_set current_sockets;
	fd_set ready_sockets;
	fd_set listen_sockets;
	bool checkRequest(int socket);
	std::map<int, Client *> client_map;
	//Request server_req;
public:
	//Server();
	Server(const std::vector<ServerConfig> &obj);
	//Server(std::vector<ServerBlock> &serveBlock);
	~Server();
	//Server &operator=(const Server &obj);
	void initServer();
	void startServer();
	void createSocket();
	void identifySocket(int PORT);
	void checkClient();
	void acceptNewConnection(int listen_sockets);
	void closeSocket();
	void shutdownServer();
};

#endif
