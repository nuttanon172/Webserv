// #include <stdio.h>
// #include <fcntl.h>
// #include <iostream>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <sstream>
// #include <sys/wait.h>
#include "cgi.hpp"

CGI::CGI(){
}

CGI::CGI(std::string interpeter, std::string script_file){
	this->interpeter = interpeter;
	this->script_file = script_file;
}

CGI::~CGI(){
}

void CGI::set_script(std::string script_file){
	this->script_file = script_file;
}

void CGI::set_interpeter(std::string interpeter){
	this->interpeter = interpeter;
}

void CGI::set_requestMethod(std::string method){
	this->requestMethod = method;
}

void CGI::set_currentDirectory(std::string directory){
	this->directory = directory;
}

void CGI::set_contentLenght(std::string contentLenght){
	this->contentLenght = contentLenght;
}

void CGI::set_contentType(std::string contentType){
	this->contentType = contentType;
}
void CGI::set_body(std::string body){
	this->body = body;
}

void CGI::init_cgi(Client clientreq){
	this->set_requestMethod(clientreq.getRequest()->getMethod());
	this->set_currentDirectory(clientreq.getRequest()->getPath());
	this->set_contentLenght(clientreq.getRequest()->getHeaderMap()["Content-Length"]);
	this->set_contentType(clientreq.getRequest()->getHeaderMap()["Content-Type"]);
	this->set_body(clientreq.getRequest()->getBody().str());
}

std::string CGI::get_script() const{
	return("SCRIPT_FILENAME=" + this->script_file);
}

std::string CGI::get_intepeter() const{
	return(this->interpeter);
}

std::string CGI::get_request_method() const{
	if(this->requestMethod == "DELETE")
		return("REQUEST_METHOD=POST");
	return("REQUEST_METHOD="+this->requestMethod);
}

std::string CGI::get_body() const{
	return(this->body);
}

std::string CGI::get_current_directory() const{
	if(this->requestMethod == "DELETE")
	{
		return("QUERY_STRING=dir=" + this->get_current_directory()+ "&" +this->get_body());
	}
	else
		return("QUERY_STRING=dir=" + this->get_current_directory());
}

std::string CGI::get_contentType() const{
	return("CONTENT_TYPE="+this->contentType);
}

std::string CGI::get_contentLenght() const{
	return("CONTENT_LENGTH="+ this->get_contentLenght());
}

std::string CGI::get_status() const{
	return("REDIRECT_STATUS=200");
}

char *const *CGI::gen_env(){
	char *const envp[] = {
	(char *)this->get_status().c_str(),
	(char *)this->get_request_method().c_str(),
	(char *)this->get_current_directory().c_str(),
	(char *)this->get_contentType().c_str(),
	(char *)this->get_contentLenght().c_str(),
	(char *)this->get_script().c_str(),
	nullptr
	};
	return(envp);
}

int write_in_chunks(int fd, const std::string& data) {
    size_t total_size = data.size();
    size_t offset = 0;

    while (offset < total_size) {
        size_t chunk_size = CHUNK_SIZE < total_size - offset ? CHUNK_SIZE: total_size - offset;
        ssize_t bytes_written = write(fd, data.c_str() + offset, chunk_size);

        if (bytes_written != chunk_size) {
            perror("write");
            return 1; // Return an error code
        }

        offset += bytes_written;
    }
    return 0; // Success
}

// template<typename T>
std::string cgi() {

	CGI cgi("/usr/bin/php-cgi", "/data/index.php");
	if (pipe(cgi.pipeid) == -1 || pipe(cgi.pipefd) == -1) {
		perror("pipe");
	}
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
	}

	if (pid == 0) { // Step 5: Fork a Child Process
			// Child process
		dup2(cgi.pipefd[0], STDIN_FILENO);
		dup2(cgi.pipeid[1], STDOUT_FILENO);
		close(cgi.pipeid[0]);
		close(cgi.pipeid[1]); // Close write end of the pipe in child
		close(cgi.pipefd[0]);
		close(cgi.pipefd[1]);

		char* const args[] = {const_cast<char*>(cgi.get_intepeter().c_str()), nullptr};
		if (execve(cgi.get_intepeter().c_str(), args, cgi.gen_env()) == -1) {
			perror("execve");
			exit(EXIT_FAILURE);
		}
	} else { // Step 7: Handle Response in Parent Process
		close(cgi.pipeid[1]);
		close(cgi.pipefd[0]);
		if (cgi.get_request_method() == "REQUEST_METHOD=POST")
		{
			if(write_in_chunks(cgi.pipefd[1], cgi.get_body()))
			{
				perror("write");
				return 1;
			}
		}
		close(cgi.pipefd[1]); // Close write end after writing
		std::stringstream stream;
		char buffer[4096];
		ssize_t nbytes;
		while ((nbytes = read(cgi.pipeid[0], buffer, sizeof(buffer))) > 0) {
			stream.write(buffer, nbytes);
		}
		close(cgi.pipeid[0]); // Close read end after reading
		int status;
		waitpid(pid, &status, 0);
		std::cout << std::endl;
		std::string response_body = stream.str();
		return response_body;
		// std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + response_body;
		// send(CLIENT.socket, http_response.c_str(), http_response.size(), 0);
	}
	return 0;
}


