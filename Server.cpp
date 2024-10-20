#include "Server.hpp"

Server::Server(const std::string &pathConfig) : max_socket(0)
{
	if (!parseConfigFile(pathConfig, serverBlock))
	{
		std::cout << "Failed to parse config file" << std::endl;
		exit(EXIT_FAILURE);
	}
	// printConfig(serverBlock);
}

Server::Server(const Server &obj)
{
	*this = obj;
}

Server &Server::operator=(const Server &obj)
{
	if (this != &obj)
	{
		server_fd = obj.server_fd;
		new_socket = obj.new_socket;
		max_socket = obj.max_socket;
		address = obj.address;
		serverBlock = obj.serverBlock;
		socketVec = obj.socketVec;
		current_sockets = obj.current_sockets;
		ready_sockets = obj.ready_sockets;
		listen_sockets = obj.listen_sockets;
		server_config = obj.server_config;
		client_map = obj.client_map;
		server_name = obj.server_name;
	}
	return *this;
}

Server::~Server()
{
	std::map<int, Client *>::iterator it = client_map.begin();
	for (; it != client_map.end(); it++)
		delete it->second;
	waitpid(-1, NULL, WNOHANG);
	this->shutdownServer();
}

void Server::startServer()
{
	FD_ZERO(&current_sockets); // init set
	FD_ZERO(&ready_sockets);
	FD_ZERO(&listen_sockets);
	if (this->initSocket() == true)
		this->checkClient();
}

bool Server::initSocket()
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
				continue;
			}
			// Enable address reuse
			if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			{
				perror("setsockopt(SO_REUSEADDR) failed");
				close(server_fd);
				continue;
			}
			// SET SOCKET NONBLOCK
			if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
			{
				perror("NONBLOCK ERROR");
				close(server_fd);
				continue;
			}
			if (this->identifySocket(*port_it, *serverBlock_it) == false)
				return false;
		}
	}
	return true;
}

bool Server::identifySocket(int port, ServerConfig &serverBlock)
{
	struct sockaddr_in address;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET; // ipv4
	inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
	address.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return perror("bind failed"), close(server_fd), false;
	if (listen(server_fd, BACKLOG) < 0)
		return perror("Listen ERROR"), close(server_fd), false;
	FD_SET(server_fd, &listen_sockets); /* add new socket to set */
	FD_SET(server_fd, &current_sockets);
	if (server_fd > max_socket)
		max_socket = server_fd;
	server_config.insert(std::make_pair(server_fd, serverBlock));
	server_name.insert(std::make_pair(server_fd, serverBlock.server_name + ":" + ft_itos(port)));
	std::cout << GREEN << "Create socket[" << server_fd << "]: http://" << server_name[server_fd] << '\n'
			  << DEFAULT;
	std::cout << YELLOW << "Max Socket: " << max_socket << '\n'
			  << DEFAULT;
	return true;
}

void Server::checkClient()
{
	int status;
	pid_t pid;

	std::cout << BLUE << "Webserver is running...\n"
			  << DEFAULT;
	while (1)
	{
		ready_sockets = current_sockets;
		status = select(max_socket + 1, &ready_sockets, NULL, NULL, NULL);
		if (status < 0)
		{
			perror("Select Error");
			std::raise(SIGINT);
		}
		for (int socket = 0; socket <= max_socket; socket++)
		{
			if (FD_ISSET(socket, &ready_sockets))
			{
				if (FD_ISSET(socket, &listen_sockets))
					this->acceptNewConnection(socket);
				else
				{
					pid = fork();
					if (pid < 0)
					{
						client_map[socket]->getResponse()->buildHttpCode(500, socket);
						this->closeSocket(socket);
					}
					else if (pid == 0)
					{
						client_map[socket]->updateTime();
						if (this->readRequest(socket) == true)
							client_map[socket]->buildResponse();
						else
							client_map[socket]->getResponse()->buildStatusLine(400);
						std::raise(SIGINT);
					}
					std::cout << YELLOW << "Webserver waiting for client....\n"
							  << DEFAULT;
					this->closeSocket(socket);
					waitpid(-1, NULL, WNOHANG);
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
		return;
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("FCNTL ERROR");
		close(new_socket);
		return;
	}
	client_map[new_socket] = new Client(new_socket, &server_config[listen_sockets]);
	FD_SET(new_socket, &current_sockets); // Accept New Connection from client
	if (new_socket > max_socket)
		max_socket = new_socket;
	client_map[new_socket]->setServerName(server_name[listen_sockets]);
	// printServerConfig(server_config[listen_sockets]);
	std::cout << GREEN << "Accept new socket: [" << new_socket << "]\n"
			  << DEFAULT;
	std::cout << GREEN << "Max Socket: " << max_socket << '\n'
			  << DEFAULT;
}

bool Server::readRequest(int socket)
{
	char buffer[100000];
	int size;
	time_t start_time;

	start_time = time(NULL);
	std::cout << "---------------------- Request ----------------------\n";
	while (true)
	{
		size = recv(socket, buffer, BUFFER_SIZE - 1, 0);
		if (size < 0)
			break;
		else if (!size)
			break;
		buffer[size] = '\0';
		// write to request string stream
		std::cout << DEFAULT << buffer << '\n';
		if (client_map[socket])
			client_map[socket]->getRequest()->writeStream(buffer, size);
		/* Check time each socket */
		if (time(NULL) - start_time > TIME_OUT)
		{
			std::cout << "Time out closing socket\n";
			client_map[socket]->getResponse()->buildHttpCode(408, socket);
			std::raise(SIGINT);
		}
		usleep(100000);
	}
	std::cout << "-----------------------------------------------------\n";
	return (true);
}

void Server::closeSocket(int socket)
{
	int max = 0;
	if (FD_ISSET(socket, &current_sockets))
	{
		FD_CLR(socket, &current_sockets);
		if (socket == max_socket)
		{
			for (int i = 0; i < max_socket; i++)
			{
				if (FD_ISSET(i, &current_sockets))
					max = i;
			}
			max_socket = max;
		}
		delete client_map[socket];
		client_map[socket] = NULL;
		close(socket);
	}
	client_map.erase(socket);
}

void Server::shutdownServer()
{
	for (int i = 0; i <= max_socket; i++)
	{
		if (FD_ISSET(i, &listen_sockets)) /* clean listen sockets */
		{
			close(i);
			server_config.erase(i);
			FD_CLR(i, &listen_sockets);
		}
		else if (FD_ISSET(i, &current_sockets))
			this->closeSocket(i); /* clean client sockets */
	}
}
