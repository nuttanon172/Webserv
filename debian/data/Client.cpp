#include "Client.hpp"

Client::Client(int socket, ServerConfig *paramBlocks) : last_time(time(NULL)), socket(socket), req(new Request(paramBlocks)), resp(new Response(paramBlocks)), serverBlock(paramBlocks)
{
	this->cgi = new CGI();
}

Client::~Client()
{
	delete req;
	delete resp;
	delete cgi;
}

bool Client::check_cgi()
{
	if (this->getRequest()->getReqPath().size() > 4 && this->getRequest()->getReqPath().substr(0, 5) != "/cgi/")
		return (false);
	else if (this->getRequest()->getReqPath().size() == 4 && this->getRequest()->getReqPath().substr(0, 4) != "/cgi")
		return (false);
	else if (this->getRequest()->getReqPath().size() < 4)
		return (false);

	std::vector<Location>::iterator it = serverBlock->locations.begin();
	for(; it != serverBlock->locations.end(); it++)
	{
		if (it->path.length() >= 4 && it->path.compare(it->path.length() - 4, 4, "/cgi") == 0) {
			std::cout << "path:" << it->path <<std::endl;
			std::vector<std::string>::iterator it1 = it->cgi_ext.begin();
			for(; it1 != it->cgi_ext.end(); it1++)
			{
				if (*it1 == ".php"){
					std::cout <<"cgi ext:" << *it1 <<std::endl;
					std::vector<std::string>::iterator it2 = it->cgi_path.begin();
					for(; it2 != it->cgi_path.end(); it2++)
					{
						std::cout <<"cgi check inter:" << *it2 <<std::endl;
						if(access((*it2).c_str(), X_OK) == 0)
						{
							std::cout << "cgi interpeter" << *it2 << std::endl;
							std::vector<std::string>::iterator it3 = it->index.begin();
							for(; it3 != it->index.end(); it3++)
							{
								if ((*it3).length() >= 4 && (*it3).compare((*it3).length() - 4, 4, *it1) == 0)
								{
									this->cgi->set_interpeter(*it2);
									this->cgi->set_script(this->getRequest()->makePath(it->path) + "/" + (*it3));
									std::cout << "cgi script " << this->getRequest()->makePath(it->path) + "/" + (*it3) << std::endl;
								}
							}
							return true;
						}
					}
				}
			}
        }
	}
	return false;
}

bool Client::buildResponse()
{
	std::stringstream ss;

	if (req->parseRequestLine() == false)
		return false;
	if (req->parseHttpHeaders() == false)
		return false;
	if (req->isMultipart() == true)
		req->parseBody();
	if (this->getResponse()->searchFile(this->getRequest(), socket) == true)
	{
		if (check_cgi())
			this->getResponse()->serveCGI(this->cgi->init_cgi(this->getRequest(), this->serverBlock), socket);
		else
			this->getResponse()->serveFile(getRequest()->getPath(), socket);

	}
	return true;
}

Request *Client::getRequest()
{
	return req;
}

Response *Client::getResponse()
{
	return resp;
}

void Client::updateTime()
{
	last_time = time(NULL);
}

time_t Client::getLastTime()
{
	return last_time;
}
