#include "WebServer.hpp"

Server *serverHandler;

void signalHandler(int signal) {
	(void)signal;
	serverHandler->shutdownServer();
	std::cout << GREEN << "Webserver shutting down...\n" << DEFAULT;
	exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	std::string pathConfig;
	std::vector<ServerConfig> servers;

	std::signal(SIGINT, signalHandler);
	if (ac > 2)
	{
		std::cerr << "Run: ./webserv or ./webserv \"config_file\"\n";
		return EXIT_FAILURE;
	}
	av[1] ? pathConfig = av[1] : pathConfig = "./default.config";
    if (!parseConfigFile(pathConfig, servers)) {
        std::cout << "Failed to parse config file" << std::endl;
        return 1;
    }
	printConfig(servers);
	/* Example config */
	/* Start Server here*/
	Server mainServer(servers);
	serverHandler = &mainServer;
	mainServer.startServer();
	return EXIT_SUCCESS;
}
