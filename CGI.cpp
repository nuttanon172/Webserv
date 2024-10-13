#include "CGI.hpp"

extern char **environ;

CGI::CGI()
{
}

CGI::CGI(std::string interpeter, std::string script_file)
{
	this->interpeter = interpeter;
	this->script_file = script_file;
}

CGI::~CGI()
{
}

void CGI::set_script(std::string script_file)
{
	this->script_file = script_file;
}

void CGI::set_interpeter(std::string interpeter)
{
	this->interpeter = interpeter;
}

void CGI::set_requestMethod(std::string &method)
{
	this->requestMethod = method;
}

void CGI::set_currentDirectory(std::string &directory)
{
	this->directory = directory;
}

void CGI::set_contentLenght(std::string contentLenght)
{
	this->contentLenght = contentLenght;
}

void CGI::set_contentType(std::string contentType)
{
	this->contentType = contentType;
}
void CGI::set_body(std::stringstream &body)
{
	std::stringstream ss;
	std::string tester;
	this->body << body.rdbuf();
}

std::string CGI::init_cgi(Request *clientreq)
{
	this->set_requestMethod(clientreq->getMethod());
	this->set_currentDirectory(clientreq->getReqPath());
	this->set_contentLenght(clientreq->getHeaderMap()["Content-Length"]);
	this->set_contentType(clientreq->getHeaderMap()["Content-Type"]);
	this->set_body(clientreq->getBody());
	return (this->cgi());
}

void CGI::get_script()
{
	this->env_script_file.insert(0, "SCRIPT_FILENAME=" + this->script_file);
}

const std::string &CGI::get_intepeter() const
{
	return this->interpeter;
}

std::string CGI::get_request_method()
{
	if (this->requestMethod == "DELETE")
		this->env_reqMethod.insert(0, "REQUEST_METHOD=POST");
	else
		this->env_reqMethod.insert(0, "REQUEST_METHOD=" + this->requestMethod);
	return (this->env_reqMethod);
}

void CGI::get_current_directory()
{
	if (this->requestMethod == "DELETE")
	{
		this->env_currDirec.insert(0, "QUERY_STRING=dir=" + this->directory + "&" + this->body.str());
	}
	else
	{
		this->env_currDirec.insert(0, "QUERY_STRING=dir=" + this->directory);
	}
}

void CGI::get_contentType()
{
	this->env_contType.insert(0, "CONTENT_TYPE=" + this->contentType);
}

void CGI::get_contentLenght()
{
	this->env_contLenght.insert(0, "CONTENT_LENGTH=" + this->contentLenght);
}

void CGI::get_status()
{
	this->env_status.insert(0, "REDIRECT_STATUS=200");
}

void CGI::gen_env()
{
	this->get_contentLenght();
	this->get_contentType();
	this->get_request_method();
	this->get_current_directory();
	this->get_script();
	this->get_status();
	putenv((char *)this->env_contLenght.c_str());
	putenv((char *)this->env_contType.c_str());
	putenv((char *)this->env_reqMethod.c_str());
	putenv((char *)this->env_currDirec.c_str());
	putenv((char *)this->env_script_file.c_str());
	putenv((char *)this->env_status.c_str());
	putenv((char *)this->env_reqMethod.c_str());
}

int CGI::write_in_chunks(int fd)
{
	std::string chunk;
	while (std::getline(this->body, chunk))
	{
		chunk.append("\n");
		size_t total_size = chunk.size();
		size_t offset = 0;
		while (offset < total_size)
		{
			size_t chunk_size = CHUNK_SIZE < total_size - offset ? CHUNK_SIZE : total_size - offset;
			size_t bytes_written = write(fd, chunk.c_str() + offset, chunk_size);

			if (bytes_written != chunk_size)
			{
				perror("write");
				return 1; // Return an error code
			}
			offset += bytes_written;
		}
		chunk.clear();
	}
	return 0; // Success
}

std::string CGI::cgi()
{

	if (pipe(this->pipeid) == -1 || pipe(this->pipefd) == -1)
	{
		perror("pipe");
	}
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	std::cout << "content lenght" << this->contentLenght << std::endl;
	if (pid == 0)
	{	// Step 5: Fork a Child Process
		// Child process
		dup2(this->pipefd[0], STDIN_FILENO);
		dup2(this->pipeid[1], STDOUT_FILENO);
		close(this->pipeid[0]);
		close(this->pipeid[1]); // Close write end of the pipe in child
		close(this->pipefd[0]);
		close(this->pipefd[1]);
		char *const args[] = {const_cast<char *>(this->get_intepeter().c_str()), NULL};
		this->gen_env();
		// std::cout << environ << std::endl;
		if (execve(this->get_intepeter().c_str(), args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{ // Step 7: Handle Response in Parent Process
		close(this->pipeid[1]);
		close(this->pipefd[0]);
		if (this->get_request_method() == "REQUEST_METHOD=POST")
		{
			std::cout << "body here" << this->body.str() << std::endl;
			if (write_in_chunks(this->pipefd[1]))
			{
				perror("write");
				return "Cannot write in chuck";
			}
		}
		close(this->pipefd[1]); // Close write end after writing
		std::stringstream stream;
		char buffer[4096];
		ssize_t nbytes;
		while ((nbytes = read(this->pipeid[0], buffer, sizeof(buffer))) > 0)
		{
			stream.write(buffer, nbytes);
		}
		close(this->pipeid[0]); // Close read end after reading
		int status;
		waitpid(pid, &status, 0);
		std::cout << std::endl;
		this->body.str("");
		this->body.clear();
		std::string response_body = stream.str();
		return response_body;
	}
	return 0;
}