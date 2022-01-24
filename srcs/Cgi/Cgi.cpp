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

Cgi::Cgi(const Server & src, const Parse_header & src_header)
{
	init_path(src);
	init_env(src, src_header);
}

Cgi::Cgi( const Cgi & src )
{
	*this = src;
}

/* Cgi::Cgi(std::vector<std::string> v) // recevoir ma requete deja valide et parser
{
	std::vector<std::string> v2 = v;
	std::string name_request = "GET / HTTP/1.1";
	std::string name_host = "Host: localhost:7777";
	v2.push_back(name_request);
	v2.push_back(name_host);
	std::vector<std::string>::iterator it;
	for (it = v2.begin(); it != v2.end(); it++)
		std::cout << "*it = " << *it << std::endl;
}
 */
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

std::string	Cgi::to_string(size_t nb)
{
	std::string str = static_cast<std::ostringstream*>( &(std::ostringstream() << nb))->str();//get_actual_listen(buff);
	return (str);
}

bool	Cgi::is_file_cgi(std::string path_extension)
{
	//if (path_extension.compare("html") == 0)
	//return (FALSE);
	return (TRUE);
}

void	Cgi::init_path(const Server & src)
{
	this->_path_cgi = "/usr/bin/php-cgi";
}

// var from client
void	Cgi::init_env_client_var(const Server & src, const Parse_header & src_header)
{
	std::string user_agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/80.0.3987.162 Safari/537.36";
	std::string accept_language = "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7,ru;q=0.6";

	this->_env["HTTP_ACCEPT"] = src_header.get_accept();
	this->_env["HTTP_ACCEPT_LANGUAGE"] = accept_language;
	this->_env["HTTP_USER_AGENT"] = user_agent;
	this->_env["HTTP_COOKIE"] = "PHPSESSID=298zf09hf012fh2; csrftoken=u32t4o3tb3gg43; _gat=1";
	this->_env["HTTP_REFERER"] = "https://developer.mozilla.org/fr/docs/Web/JavaScript";
}

// var server
void	Cgi::init_env_server_var(const Server & src, const Parse_header & src_header)
{
	std::set<std::string>::iterator it;
	it = src.getName().begin();
	//this->_env["SERVER_SOFTWARE"] = "localhost:8001/1.0";
	//this->_env["SERVER_NAME"] = *it;
	//this->_env["GATEWAY_INTERFACE"] = "/usr/bin/php-cgi/7.2";
}

// var request
void	Cgi::init_env_request_var(const Server & src, const Parse_header & src_header)
{
	this->_env["AUTH_TYPE"] = "HTTP";
	this->_env["PATH_INFO"] = src.getRoot() + "/hello.php"; // P_INFO + QUERY STRING = REQUEST URI
	//this->_env["PATH_TRANSLATED"] = "";
	this->_env["QUERY_STRING"] = "a=b";
	this->_env["REQUEST_URI"] = src.getRoot() + "/hello.php?a=b";
	//this->_env["REQUEST_METHOD"] = src_header.get_method(); // pas bien
	//this->_env["REMOTE_HOST"] = "127.0.0.1:8001";
	this->_env["SCRIPT_FILENAME"] = this->_path_cgi;
	this->_env["SERVER_PORT"] = src.getListen();
	this->_env["SERVER_PROTOCOL"] = src_header.get_protocol();
	this->_env["REDIRECT_STATUS"] = to_string(src_header.get_request_status());
	this->_env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
}

void	Cgi::init_env(const Server & src, const Parse_header & src_header)
{
	std::map<std::string, std::string>::iterator it_env;

	init_env_client_var(src, src_header);
	init_env_server_var(src, src_header);
	init_env_request_var(src, src_header);
	for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
		std::cout << PURPLE << it_env->first << " = " << BLUE << it_env->second << std::endl << END;
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

char	**Cgi::create_argv(std::string path_cgi, std::string path_file_executed)
{
	int		nbr_argv = 2;
	char	**argv = new char *[nbr_argv + 1];

	std::string a = path_cgi; // useless arg
	argv[0] = new char[1];
	strcpy(argv[0], "");
	argv[1] = new char[path_file_executed.size() + 1];
	strcpy(argv[1], path_file_executed.c_str());
	argv[2] = NULL;
	return (argv);
}

void	Cgi::exec_cgi(const Server & src, char **argv, char **env)
{
	(void)src;
	int i = 0, fd_out = 0, status = 0;
	int pipefd[2];

	pipe(pipefd);
	this->_pid = fork();
	if (this->_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (execve(this->_path_cgi.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
	}
	waitpid(this->_pid, &status, 0);
	close(pipefd[1]);
	this->_send_content = fd_to_string(pipefd[0]);
	delete_argv_env(argv, env);
	//std::cout << GREEN << "_send_content = " << std::endl << "|" <<
	//this->_send_content << "|" << std::endl << END;
}


std::string	Cgi::fd_to_string(int fd)
{
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::in);
    std::istream is(&filebuf);
	std::string ret, line;

	ret = "HTTP/1.1 200 OK\n";
	while (std::getline(is, line))
	{
		ret += line;
		ret += '\n';
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

int	Cgi::getPid() const
{
	return (this->_pid);
}
/* ************************************************************************** */
