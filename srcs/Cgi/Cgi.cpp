/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/18 15:34:51 dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi(std::string const & root, std::string const & path, std::string const & pathCgi, const Parse_request & src_header, const Engine & src_engine)
{
	init_path(root, path, pathCgi);
	init_env(src_header, src_engine);
}

Cgi::Cgi( const Cgi & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cgi::~Cgi()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cgi &				Cgi::operator=( Cgi const & rhs )
{
	if ( this != &rhs )
	{
		this->_path_cgi = rhs.getPath_cgi();
		this->_send_content = rhs.getSend_content();
		this->_env = rhs.getEnv();
		this->_pid = rhs.getPid();
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Cgi const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Cgi::delete_argv_env(char **argv, char **env)
{
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	for (int i = 0; argv[i]; i++)
		delete [] argv[i];
	delete [] env;
	delete [] argv;
}

void	Cgi::init_path(std::string const & root, std::string const & path, std::string const & pathCgi)
{
	this->_path_file_executed = path;
	this->_path_cgi = pathCgi;
	this->_root = root;
	this->_path_file_executed_absolu =
		this->_path_file_executed.substr(this->_root.size(), this->_path_file_executed.size());
	this->_user = "user42";
	//this->_home = "/home/user42/Bureau/webserv/www";
}

// var from client
void	Cgi::init_env_client_var(const Parse_request & src_header)
{
	this->_env["HTTP_ACCEPT"] = src_header.get_request("Accept:");
	this->_env["HTTP_ACCEPT_LANGUAGE"] = src_header.get_request("Accept-Language:");
	this->_env["HTTP_USER_AGENT"] = src_header.get_request("User-Agent:");
	this->_env["HTTP_CONNECTION"] = src_header.get_request("Connection:");
	if (src_header.get_request("Method").compare("POST") == 0)
	{
		this->_env["HTTP_CONTENT_LENGTH"] = src_header.get_request("Content-Length:");
		this->_env["HTTP_CONTENT_TYPE"] = src_header.get_request("Content-Type:");
	}
	this->_env["HTTP_REFERER"] = src_header.get_request("Referer:");
}

// var server
void	Cgi::init_env_server_var(const Parse_request & src_header)
{
	this->_env["HOME"] = this->_home; // pas sur
	this->_env["USER"] = this->_user; // pas sur
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	//this->_env["SERVER_NAME"] = src_header.get_request("Host:");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

// var request
void	Cgi::init_env_request_var(const Parse_request & src_header, const Engine & src_engine)
{
	/* difference entre GET et POST sur un form html qui redirect ur env.php:
		POST par rapport a get: + : HTTP_CONTENT_LENGTH, HTTP_CONTENT_TYPE == CONTENT_TYPE et CONTENT_LENGTH
		- : QUERY_STRING vide et du coup REQUEST_URI vide
	*/
	// remplacer "/env.php" par le bon fichier apres traitement de requete:
	//this->_env["AUTH_TYPE"] = "HTTP";
	//this->_env["REQUEST_SCHEME"] = "http";
	if (src_header.get_request("Query").compare(""))
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu + '?' + src_header.get_request("Query");//query string ;
	else
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu;
	this->_env["SCRIPT_FILENAME"] = this->_path_file_executed;
	this->_env["DOCUMENT_ROOT"] = this->_root;
	this->_env["DOCUMENT_URI"] = this->_path_file_executed_absolu;
	this->_env["SERVER_PROTOCOL"] = src_header.get_request("Protocol");
	this->_env["SERVER_PORT"] = int_to_string(src_engine.GetAccessPort());
	this->_env["REQUEST_METHOD"] = src_header.get_request("Method"); // pas bien
	this->_env["SCRIPT_NAME"] = this->_path_file_executed_absolu;
	this->_env["QUERY_STRING"] = src_header.get_request("Query");
	this->_env["REMOTE_PORT"] = src_engine.GetRemote_Port();
	this->_env["REMOTE_ADDR"] = src_engine.GetRemote_Addr();
	this->_env["CONTENT_TYPE"] = src_header.get_request("Content-Type:");
	this->_env["CONTENT_LENGTH"] = src_header.get_request("Content-Length:");
	this->_env["REDIRECT_STATUS"] = src_header.get_request("Status");
}

void	Cgi::init_env(const Parse_request & src_header, const Engine & src_engine)
{
	std::map<std::string, std::string>::iterator it_env;

	init_env_client_var(src_header);
	init_env_server_var(src_header);
	init_env_request_var(src_header, src_engine);
	//for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
		//std::cout << PURPLE << it_env->first << " = " << BLUE << it_env->second << std::endl << END;
}

char **Cgi::convert_env(std::map<std::string, std::string>)
{
	std::map<std::string, std::string>::const_iterator it_env;
	char	**env = new char *[this->_env.size() + 1];
	int	j = 0;
	for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
	{
		std::string	content = it_env->first + "=" + it_env->second;
		env[j] = new char[content.size() + 1];
		env[j] = strcpy(env[j], content.c_str());
		j++;
	}
	env[j] = NULL;
	return (env);
}

char	**Cgi::create_argv(std::string path_file_executed)
{
	int		nbr_argv = 2;
	char	**argv = new char *[nbr_argv + 1];

	argv[0] = new char[1];
	strcpy(argv[0], "");
	argv[1] = new char[path_file_executed.size() + 1];
	strcpy(argv[1], path_file_executed.c_str());
	argv[2] = NULL;
	return (argv);
}

void	Cgi::write_body_post_in_fd(std::string body_string) // body | php-cgi
{
	std::cout << RED << "body_string = " << body_string << END << std::endl ;
	int fds_child[2];

	pipe(fds_child);
	dup2(fds_child[0], STDIN_FILENO);
	close(fds_child[0]);
	write(fds_child[1], body_string.c_str(), sizeof(body_string));
	close(fds_child[1]);
}

void	Cgi::exec_cgi(char **argv, char **env, const Parse_request & src_header)
{
	std::string body_string = src_header.get_request_body();
	int i = 0, fd_out = 0, status = 0;
	int fds_exec[2];

	pipe(fds_exec);
	this->_pid = fork();
	if (this->_pid == 0)
	{
		if (src_header.get_request("Method").compare("POST") == 0)
			write_body_post_in_fd(body_string); // for post request
		dup2(fds_exec[1], STDOUT_FILENO);
		close(fds_exec[0]);
		close(fds_exec[1]);
		if (execve(this->_path_cgi.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
	}
	waitpid(this->_pid, &status, 0);
	close(fds_exec[1]);
	this->_send_content = body_response_from_fd(fds_exec[0]);
	close(fds_exec[0]);
	delete_argv_env(argv, env);
	std::cout << GREEN << "_send_content = " << std::endl << "|" <<
	this->_send_content << "|" << std::endl << END;
}

std::string	Cgi::body_response_from_fd(int fd)
{
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::in);
    std::istream is(&filebuf);
	std::string ret, line;
	std::string content_type = "Content-type:";
	while (std::getline(is, line))
	{
		if (line.compare(0, content_type.size(), content_type) == 0)
			this->_type_cgi = line;
		else
		{
			ret += line;
			ret += '\n';
		}
	}
	return (ret);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::map<std::string, std::string> Cgi::getEnv() const
{
	return (this->_env);
}

std::string	Cgi::getSend_content() const
{
	return (this->_send_content);
}

std::string	Cgi::getPath_cgi() const
{
	return (this->_path_cgi);
}

std::string	Cgi::getUser() const
{
	return (this->_user);
}

std::string	Cgi::getHome() const
{
	return (this->_home);
}

int	Cgi::getPid() const
{
	return (this->_pid);
}

std::string	Cgi::getType_Cgi() const
{
	return (this->_type_cgi);
}
/* ************************************************************************** */
