#include "WebServer.hpp"

Server *serverHandler;
std::string *path;

void signalHandler(int signal) {
	(void)signal;
	serverHandler->shutdownServer();
	std::cout << RED << "\nWebserver shutting down...\n" << DEFAULT;
	delete serverHandler;
	delete path;
	exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	std::string *pathConfig = new std::string;

	path = pathConfig;
	std::signal(SIGINT, signalHandler);
	if (ac > 2)
	{
		std::cerr << "Run: ./webserv or ./webserv \"config_file\"\n";
		return EXIT_FAILURE;
	}
	av[1] ? *pathConfig = av[1] : *pathConfig = "./default.config";
	/* Example config */
	/* Start Server here*/
	Server *mainServer = new Server(*pathConfig);
	serverHandler = mainServer;
	mainServer->startServer();
	return EXIT_SUCCESS;
}
