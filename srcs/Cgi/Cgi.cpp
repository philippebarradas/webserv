/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/17 13:17:50 by dodjian          ###   ########.fr       */
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
	initEnv(src);
	exec_cgi(src);
}

Cgi::Cgi( const Cgi & src )
{
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Cgi const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Cgi::initEnv(const Server & src)
{
	std::map<std::string , std::string>::iterator it_cgi;

	it_cgi = src.getCgi().begin();
	std::string	nameVar[] = {"REQUEST_METHOD", "SCRIPT_FILENAME", "SERVER_PORT"};
	std::string	valueVar[] = {"GET", it_cgi->second.c_str(), "7777"};

	for (size_t i = 0; i < 3; i++)
		this->_env.insert(std::make_pair(nameVar[i], valueVar[i]));
}

char **Cgi::Convert_env(std::map<std::string, std::string>)
{
	std::map<std::string, std::string>::iterator it_env;
	char **env = new char *[this->_env.size() + 1];
	int j = 0;
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

void	Cgi::exec_cgi(const Server & src)
{
	char *const *nul = NULL;
	char **env;

	env = this->Convert_env(this->_env);
	int i = 0;
	while (env[i] != NULL)
	{
		std::cout << "env[i] = " << env[i] << std::endl;
		i++;
	}
	std::string a = "feefw";
	std::string b = "/home/user42/Bureau/webserv/srcs/Config/default/html_page/hello.php";
	int size = 10;
	char **argv = create;

	argv[0] = strdup(a.c_str());
	argv[1] = strdup(b.c_str());
	argv[2] = 0;
	int j = 0;
	std::cout << "fewefwe" << std::endl;
	while (argv[j])
	{
		std::cout << "argv[j] = " << argv[j] << std::endl;
		j++;
	}
	std::string exec_path = "/usr/bin/php";
	//std::string path_cgi = "/usr/bin/python3";
	//std::string path_file = " /home/user42/Bureau/webserv/srcs/Config/default/html_page/cgi_python.py";
	//exec_path += " /home/user42/Bureau/webserv/srcs/Config/default/html_page/hello.php";
	std::cout << exec_path << std::endl;
	this->_pid = fork();
	if (this->_pid == 0)
	{
		if (execve(exec_path.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
		//redirect_result_cgi();
	}
}

void	Cgi::redirect_result_cgi()
{
	std::string line;
	std::string ret;
	while (std::getline(std::cin, line))
	{
		ret += line;
		ret += '\n';
		std::cout << "ret = " << ret;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::map<std::string, std::string> Cgi::getEnv() const
{
	return (this->_env);
}

/* ************************************************************************** */
