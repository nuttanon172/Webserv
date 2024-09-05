#include "Server.hpp"

Server::Server(const std::vector<ServerConfig> &obj) : serverBlock(obj)
{

}

Server::~Server()
{
}

Server &Server::operator=(const Server &obj)
{
	return *this;
}

void Server::initServer()
{
	max_socket = 0;
}

void Server::mainSever()
{
	FD_ZERO(&current_sockets); // init set
	FD_ZERO(&ready_sockets);
	FD_ZERO(&listen_sockets);
	createSocket();
	//checkClient();
}

void Server::createSocket()
{
	int enable = 1;

	// Socket for each port
	std::vector<ServerConfig>::iterator serverBlock_it = serverBlock.begin();
	std::vector<int>::iterator port_it;
	for (; serverBlock_it != serverBlock.end(); serverBlock_it++)
	{
		port_it = serverBlock_it->listen_ports.begin();
		for (; port_it != serverBlock_it->listen_ports.end(); port_it++)
		{
			server_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (server_fd < 0)
			{
				perror("Cannot create socket");
				exit(EXIT_FAILURE);
			}
			// Enable address reuse
			if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			{
				perror("setsockopt(SO_REUSEADDR) failed");
			}
			// SET SOCKET NONBLOCK
			if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
			{
				perror("NONBLOCK ERROR");
				close(server_fd);
				exit(EXIT_FAILURE);
			}
			identifySocket(*port_it);
		}
	}
}

void Server::identifySocket(int PORT)
{
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("Listen ERROR");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	FD_SET(server_fd, &listen_sockets); /* add new socket to set */
	FD_SET(server_fd, &current_sockets);
	if (server_fd > max_socket)
		max_socket = server_fd;
}

void Server::checkClient()
{
	while (1)
	{
		ready_sockets = current_sockets; /* because select is destructive */
		if (select(max_socket + 1, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			perror("Select Error");
			exit(EXIT_FAILURE);
		}
		for (int socket = 0; socket < max_socket; socket++)
		{
			if (FD_ISSET(socket, &ready_sockets))
			{
				if (FD_ISSET(socket, &listen_sockets))
					this->acceptNewConnection(socket);
				else
				{
					//client_map[socket]->updateTime();
					//if (time(NULL) - client_map[socket]->getLastTime() > 5)
					//	exit(0); // time out client
					//if (this->checkRequest(socket))
					//	exit(0);
				}
			}
		}
	}
}

void Server::acceptNewConnection(int listen_sockets)
{
	int addrlen = sizeof(address);
	new_socket = accept(listen_sockets, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket < 0)
	{
		perror("Accept ERROR");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("NONBLOCK ERROR");
		close(server_fd);
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	//client_map[new_socket] = new Client(new_socket, listen_sockets, serveBlock);
	FD_SET(new_socket, &current_sockets); // Accept New Connection from client
	if (new_socket > max_socket)
		max_socket = new_socket;
	std::cout << "new_socket = " << new_socket << '\n';
	std::cout << "max_socket = " << max_socket << '\n';
	std::cout << "Recieve Request...\n";

}

bool Server::checkRequest(int socket)
{
	char buffer[10000];
	int size;

	while (true)
	{
		size = recv(socket, buffer, sizeof(buffer), 0);
		if (size < 0)
			break ;
		else if (!size)
			return (false); // no message -- close connection
		buffer[size] = '\0';
		// write to request string stream
		//client_map[new_socket]->getRequest().writeStream(buffer, size);
	}	
	return (true);
}

void Server::closeSocket()
{
	close(server_fd);
	close(new_socket);
}
