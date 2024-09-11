#include "Server.hpp"

/*Server::Server(const std::vector<ServerConfig> &obj) : serverBlock(obj)
{
	initServer();
}*/

Server::Server(const std::string &pathConfig)
{
	if (!parseConfigFile(pathConfig, serverBlock)) {
        std::cout << "Failed to parse config file" << std::endl;
        exit(EXIT_FAILURE);
    }
	printConfig(serverBlock);
	initServer();
}

Server::~Server()
{
	std::map<int, Client *>::iterator it = client_map.begin();
	for (; it != client_map.end(); it++)
		delete it->second;
}

void Server::initServer()
{
	max_socket = 0;
}

void Server::startServer()
{
	FD_ZERO(&current_sockets); // init set
	FD_ZERO(&ready_sockets);
	FD_ZERO(&listen_sockets);
	initSocket();
	checkClient();
}

void Server::initSocket()
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
			identifySocket(*port_it, *serverBlock_it);
		}
	}
}

void Server::identifySocket(int port, ServerConfig &serverBlock)
{
	struct sockaddr_in address;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET; // ipv4
	if (serverBlock.host == "127.0.0.1" || serverBlock.host == "0.0.0.0")
		address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
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
	config_map[server_fd] = &serverBlock;
	// socketVec.push_back(server_fd);
	std::cout << GREEN << "create socket[" << server_fd << "]\n"
			  << DEFAULT;
}

void Server::checkClient()
{
	int status;
	int new_max;

	while (1)
	{
		memcpy(&ready_sockets, &current_sockets, sizeof(current_sockets)); /* because select is destructive */
		status = select(max_socket + 1, &ready_sockets, NULL, NULL, NULL);
		std::cout << "waiting...\n";
		if (status < 0)
		{
			perror("Select Error");
			exit(EXIT_FAILURE);
		}
		for (int socket = 0; socket <= max_socket; socket++)
		{
			std::cout << "Socket: " << socket << '\n';
			std::cout << "Max_Socket: " << max_socket << '\n'
					  << '\n';
			if (FD_ISSET(socket, &ready_sockets))
			{
				std::cout << "Select socket: " << socket << '\n';
				if (FD_ISSET(socket, &listen_sockets))
					this->acceptNewConnection(socket);
				else
				{
					std::cout << "Sending to socket: " << socket << "\n";
					checkRequest(socket);
					client_map[socket]->buildResponse();
					// std::string filename = "/home/ntairatt/WebServ/docs/fusion_web/index.html";
					// client_map[socket]->getResponse()->serveFile(filename, socket);
					if (FD_ISSET(socket, &current_sockets))
					{
						FD_CLR(socket, &current_sockets);
						new_max = 0;
						if (socket == max_socket)
						{
							for (int i = 0; i < max_socket; i++)
							{
								if (FD_ISSET(i, &current_sockets))
									new_max = i;
							}
							max_socket = new_max;
							close(socket);
						}
						delete client_map[socket];
						client_map.erase(socket);
					}
					status--;
					/* Check time each socket */
					/*std::map<int, Client *>::iterator it = client_map.begin();
					for (it; it != client_map.end(); it++)
					{
						if (time(NULL) - it->second->getLastTime() > 5)
						{
							if (FD_ISSET(socket, &current_sockets))
								FD_CLR(socket, &current_sockets);
							new_max = 0;
							if (it->first == max_socket)
							{
								for (int i = 0; i < max_socket; i++)
								{
									if (FD_ISSET(i, &current_sockets))
										new_max = i;
								}
								max_socket = new_max;
							}
							close(it->first);
							std::cout << YELLOW << "Socket [" << it->first << "] closed\n";
							client_map.erase(it->first);
						}
					}*/
					/* Display socket value */
					std::cout << "status: " << status << ", message sent\n";
					std::cout << "Max_Socket: " << max_socket << '\n'
							  << '\n';
					std::cout << YELLOW << "Webserver waiting for client....\n"
							  << DEFAULT;
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
	client_map[new_socket] = new Client(new_socket, serverBlock);
	FD_SET(new_socket, &current_sockets); // Accept New Connection from client
	if (new_socket > max_socket)
		max_socket = new_socket;
	std::cout << GREEN << "accept socket[" << new_socket << "]\n"
			  << DEFAULT;
	std::cout << "new_socket = " << new_socket << '\n';
	std::cout << "max_socket = " << max_socket << '\n';
	std::cout << "Recieve Request...\n";
}

bool Server::checkRequest(int socket)
{
	char buffer[10000];
	int size;

	client_map[socket]->updateTime();
	while (true)
	{
		size = recv(socket, buffer, sizeof(buffer), 0);
		std::cout << "size: " << size << '\n';
		if (size < 0)
			break;
		else if (!size)
			return (false); // no message -- close connection
		buffer[size] = '\0';
		// write to request string stream
		std::cout << buffer << '\n';
		client_map[new_socket]->getRequest()->writeStream(buffer, size);
	}
	return (true);
}

void Server::closeSocket()
{
	std::vector<int>::iterator it = socketVec.begin();
	for (; it != socketVec.end(); it++)
	{
		FD_CLR(*it, &listen_sockets);
		FD_CLR(*it, &current_sockets);
		close(*it);
		std::cout << RED << "socket[" << *it << "] closed...\n"
				  << DEFAULT;
	}
}

void Server::shutdownServer()
{
	closeSocket();
}
