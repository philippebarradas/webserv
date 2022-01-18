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
	init_env(src, src_header);
	char **env = this->convert_env(this->_env);
	char **argv = this->create_argv();
	exec_cgi(src, argv, env);
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	for (int i = 0; argv[i]; i++)
		delete [] argv[i];
	delete [] env;
	delete [] argv;
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
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
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

std::string	Cgi::to_string(size_t nb)
{
	std::string str = static_cast<std::ostringstream*>( &(std::ostringstream() << nb))->str();//get_actual_listen(buff);
	return (str);
}

bool	Cgi::is_cgi(const Parse_header & src)
{
	std::cout << "path = |" << src.get_path() << "|" << std::endl;
	return (TRUE);
}

void	Cgi::init_env(const Server & src, const Parse_header & src_header)
{
	std::string user_agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/80.0.3987.162 Safari/537.36";
	std::string accept_language = "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7,ru;q=0.6";
	std::string http_accept = "text/*";
	std::string content_type = "application/x-www-form-urlencoded";
	std::string path_to_cgi_script = "/usr/bin/";
	std::map<std::string, std::string>::iterator it_cgi, it_env;

	it_cgi = src.getCgi().begin();
	std::string script_filename = path_to_cgi_script + it_cgi->second.c_str();

	// var client
	this->_env["HTTP_ACCEPT"] = src_header.get_accept();
	//this->_env["HTTP_ACCEPT_LANGUAGE"] = src_header.get_accept_language();
	this->_env["HTTP_ACCEPT_LANGUAGE"] = accept_language;
	this->_env["HTTP_USER_AGENT"] = user_agent;
	//this->_env["HTTP_USER_AGENT"] = src_header.get_user_agent();
	//this->_env["HTTP_COOKIE"] = "";
	//this->_env["HTTP_REFERER"] = "";
	// var server
	this->_env["PATH_INFO"] = src_header.get_path();
	//this->_env["PATH_TRANSLATED"] = "";
	this->_env["REQUEST_METHOD"] = src_header.get_method();
	this->_env["REMOTE_HOST"] = src_header.get_host();
	this->_env["REMOTE_HOST"] = host;
	this->_env["SCRIPT_NAME"] = path_to_cgi_script;
	this->_env["SCRIPT_FILENAME"] = script_filename;
	this->_env["SERVER_PORT"] = src.getListen();
	this->_env["SERVER_PROTOCOL"] = src_header.get_protocol();
	this->_env["REDIRECT_STATUS"] = to_string(src_header.get_request_status());
	this->_env["CONTENT_TYPE"] = content_type;
	for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
		std::cout << PURPLE << it_env->first << " = " << BLUE << it_env->second << std::endl << END;
}

char **Cgi::convert_env(std::map<std::string, std::string>)
{
	std::map<std::string, std::string>::iterator it_env;
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

char	**Cgi::create_argv()
{
	int	nbr_argv = 2;
	char	**argv = new char *[nbr_argv + 1];

	std::string a = "/usr/bin/php-cgi";
	std::string b = "/usr/bin/";

	argv[0] = new char[a.size() + 1];
	strcpy(argv[0], a.c_str());
	argv[1] = new char[b.size() + 1];
	strcpy(argv[1], b.c_str());
	argv[2] = NULL;
	return (argv);
}

void	Cgi::exec_cgi(const Server & src, char **argv, char **env)
{
	(void)src;
	int i = 0, fd_out = 0, status = 0;
	int pipefd[2];
	std::string exec_path = "/usr/bin/php-cgi";

	pipe(pipefd);
	this->_pid = fork();
	if (this->_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (execve(exec_path.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
	}
	waitpid(this->_pid, &status, 0);
	close(pipefd[1]);
	//std::cout << GREEN << "line = " << line << END;
	this->_send_content = redirect_result_cgi(pipefd);
	std::cout << GREEN << "_send_content = |" << this->_send_content << "|" << std::endl << END;
}

std::string	Cgi::redirect_result_cgi(int pipefd[2])
{
	std::string ret;
	char buf[10000];

	bzero(buf, sizeof(buf));
	while (read(pipefd[0], buf, sizeof(buf)) > 0)
	{
		ret += buf;
		bzero(buf, sizeof(buf));
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

/* ************************************************************************** */
