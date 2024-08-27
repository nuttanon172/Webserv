#include "Server.hpp"

int main()
{
	ServerBlock serveBlock;
	Server mainServer(serveBlock);

	/* Example config */
	serveBlock._portVec = {9001, 9002};
	serveBlock._severName = "localhost";
	serveBlock._fullName = {"localhost:9001", "localhost:9002"};
	serveBlock._hostIP = 16777343;
	serveBlock._root = "docs/webA";
	serveBlock._clientMaxBodySize = 0;
	serveBlock._indexVec = {"index.html", "indexA.html"};
	serveBlock._errorMap.insert(std::pair<int, std::string>(404, "default-error/404.html"));
	serveBlock._errorMap.insert(std::pair<int, std::string>(404, "default-error/404.html"));
	serveBlock._autoIndex = true;
	/* Start Server here*/
	mainServer.mainSever();
}