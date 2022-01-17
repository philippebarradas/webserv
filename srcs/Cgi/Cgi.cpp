/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/17 19:10:44 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi(const Server & src)
{
	init_env(src);
	char **env = this->convert_env(this->_env);
	char **argv = this->create_argv();
	exec_cgi(src, argv, env);
	for (int i = 0; i < 10 && env[i]; i++)
	{
		delete [] env[i];
		delete [] argv[i];
	}
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

bool	Cgi::is_cgi(const Parse_header & src)
{
	std::cout << "path = |" << src.get_path() << "|" << std::endl;
	return (TRUE);
}

void	Cgi::init_env(const Server & src)
{
	std::map<std::string, std::string>::iterator it_cgi;

	it_cgi = src.getCgi().begin();
	std::string	nameVar[] = {"REQUEST_METHOD", "SCRIPT_FILENAME", "SERVER_PORT"};
	std::string	valueVar[] = {"GET", it_cgi->second.c_str(), "7777"};

	for (size_t i = 0; i < 3; i++)
		this->_env.insert(std::make_pair(nameVar[i], valueVar[i]));
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
	std::string nul_str = "";
	std::string a = "/usr/bin/php";
	std::string b = "/home/user42/Bureau/webserv/srcs/Config/default/html_page/hello.php";

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
	std::string exec_path = "/usr/bin/php";
	std::string path_cgi_file = "res_cgi";

	pipe(pipefd);
	this->_pid = fork();
	if (this->_pid == 0)
	{
		fd_out = open(path_cgi_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 00666);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
		if (execve(exec_path.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
		close(fd_out);
	}
	this->_send_content = redirect_result_cgi(path_cgi_file);
	waitpid(this->_pid, &status, 0);
}

std::string	Cgi::redirect_result_cgi(std::string path_cgi_file)
{
	std::ifstream ifs;
	std::string	line, ret;

	ifs.open(path_cgi_file, std::ifstream::in);
	if (!ifs.is_open())
		return (NULL);
	while (std::getline(ifs, line))
	{
		ret += line;
		ret += '\n';
	}
	//std::cout << "ret = " << ret;
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
