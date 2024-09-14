#ifndef CGI_HPP
# define CGI_HPP

# include "WebServer.hpp"
# include <sstream>
# define CHUNK_SIZE 4000
struct ServerConfig;
class Request;
class Response;
class Client;

class CGI
{
	public:
		CGI();
		CGI(std::string interpeter, std::string script_file);
		~CGI();
		int pipeid[2];
		int pipefd[2];
		void set_script(std::string script_file);
		void set_interpeter(std::string interpeter);
		// void set_redirectStatus(int status);
		void set_requestMethod(std::string method);
		void set_currentDirectory(std::string directory);
		void set_contentLenght(std::string contentLenght);
		void set_contentType(std::string contentType);
		void set_body(std::string body);
		void init_cgi(Client client);
		// void set_boudary(std::string boudary);
		std::string get_script() const;
		std::string get_intepeter() const;
		std::string get_request_method() const;
		std::string get_current_directory() const;
		std::string get_contentType() const;
		std::string get_contentLenght() const;
		std::string get_status() const;
		std::string get_body() const;
		// std::string get_boudary();
		char *const *gen_env();
		// template<typename T>
		// int cgi(T CLIENT, T REQUEST);
	private:
		// int status;
		std::string contentType;
		std::string contentLenght;
		std::string directory;
		std::string requestMethod;
		std::string interpeter;
		std::string script_file;
		std::string body;
		// std::string boudary;
};
#endif
