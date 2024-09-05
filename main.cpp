//#include "WebServer.hpp"
#include "Config.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"

int main(int ac, char **av)
{
	std::string pathToConfigFile;

	if (ac > 2)
	{
		std::cout << "./webserv or ./webserv \"config_file\"";
		return EXIT_FAILURE;
	}
	ac == 2 ? pathToConfigFile = av[1] : pathToConfigFile = "./default.conf";

	/* Example config */
	try {
	Config config(pathToConfigFile);
	config.DebugEverythinginConfig(config.getServerBlocks());
	std::vector<ServerBlock> s_vec = config.getServerBlocks();
	//Server Handler(s_vec);
	 std::vector<ServerBlock>::iterator ptr;
	for (ptr = s_vec.begin(); ptr < s_vec.end(); ptr++)
		std::cout << ptr->_root << ' ' << ptr->_serverNames << '\n';
	for (ptr = s_vec.begin(); ptr < s_vec.end(); ptr++)
		ptr->DebugServerBlock();
	/* Start Server here*/
	// Server mainServer(serveBlock);
	}
	catch (std::string &e)
	{
		std::cout << e << '\n';
	}
}
