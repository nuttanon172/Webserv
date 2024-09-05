#include "WebServer.hpp"

int main(int ac, char **av)
{
	std::string pathConfig;
	std::vector<ServerConfig> servers;

	if (ac > 2)
	{
		std::cout << "./webserv or ./webserv \"config_file\"";
		return EXIT_FAILURE;
	}
	pathConfig = "./default.config";
    if (!parseConfigFile(pathConfig, servers)) {
        std::cout << "Failed to parse config file" << std::endl;
        return 1;
    }
	printConfig(servers);
	/* Example config */
	/* Start Server here*/
	Server mainServer(servers);
}
