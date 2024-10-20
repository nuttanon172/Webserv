#ifndef CGI_HPP
#define CGI_HPP

#include "WebServer.hpp"

#define CHUNK_SIZE 4000

struct ServerConfig;
class Request;
class Response;
class Client;

class CGI
{
public:
	std::string env_reqMethod;
	std::string env_contType;
	std::string env_currDirec;
	std::string env_contLenght;
	std::string env_script_file;
	std::string env_interpeter;
	std::string env_status;
	CGI();
	CGI(std::string interpeter, std::string script_file);
	CGI(const CGI &obj);
	CGI &operator=(const CGI &obj);
	~CGI();
	std::string cgi();
	int pipeid[2];
	int pipefd[2];
	void set_script(std::string script_file);
	void set_interpeter(std::string interpeter);
	void set_requestMethod(std::string &method);
	void set_currentDirectory(std::string &directory);
	void set_contentLenght(std::string contentLenght);
	void set_contentType(std::string contentType);
	void set_body(std::stringstream &body);
	std::string init_cgi(Request *clientreq);
	const std::string &get_intepeter() const;
	std::string get_request_method();
	void get_current_directory();
	void get_contentType();
	void get_contentLenght();
	void get_status();
	int write_in_chunks(int fd);
	void gen_env();
	void get_script();
	std::string &get_interpeter();
	std::string &get_script_file();
private:
	std::string contentType;
	std::string contentLenght;
	std::string directory;
	std::string requestMethod;
	std::string interpeter;
	std::string script_file;
	std::stringstream body;
};
#endif