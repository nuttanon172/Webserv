#include "Server.hpp"

Server::Server(ServerBlock &serveBlock) : serveBlock(serveBlock)
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
	FD_ZERO(&current_sockets); //init set
	FD_ZERO(&ready_sockets);

	createSocket();
}

void Server::createSocket()
{
	int enable = 1;

	// Socket for each port
	std::vector<int>::iterator it = serveBlock.getPort().begin();
	for (; it != serveBlock.getPort().end(); it++)
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
		identifySocket(*it);
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
	FD_SET(server_fd, &current_sockets); /* add new socket to set */
}

void Server::checkClient()
{
	max_socket = server_fd;
	while (1)
	{
		ready_sockets = current_sockets; /* because select is destructive */
		if (select(max_socket + 1, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			perror("Select Error");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < max_socket; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server_fd)
				{
					int client_socket = this->acceptNewConnection();
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket)
						max_socket = client_socket;
				}
				else
				{
					_client_map[i]->updateTime();
					if (time(NULL) - _client_map[i]->getLastTime() > 5)
						max_socket--;
				}
			}
		}
	}
}

int Server::acceptNewConnection()
{
	int addrlen = sizeof(address);
	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
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
	return new_socket;
}

void Server::closeSocket()
{
	close(server_fd);
	close(new_socket);
}
