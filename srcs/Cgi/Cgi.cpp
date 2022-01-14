/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/14 18:29:30 by dodjian          ###   ########.fr       */
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
	createEnv(src);
	//exec_cgi(src);
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

void	Cgi::exec_cgi(const Server & src)
{
	std::map<std::string , std::string>::iterator it_cgi;

	it_cgi = src.getCgi().begin();
	this->pid = fork();
	execl(it_cgi->second.c_str(), "./webserv");
	if (this->pid == 0)
	{
		//std::cout << "getcgi = " << src.getCgi() << std::endl;
		//execv(str.c_str(), )
		//execl(str.c_str(), NULL);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void	Cgi::createEnv(const Server & src)
{
	std::map<std::string , std::string>::iterator it_cgi;
	std::vector<std::pair<std::string, std::string>> env;

	it_cgi = src.getCgi().begin();
	std::string	nameVar[] = {"REQUEST_METHOD", "SCRIPT_FILENAME", "SERVER_PORT"};
	std::string	valueVar[] = {"GET", it_cgi->second.c_str(), "7777"};

	//ret = this->_error.insert(std::make_pair( stoui_size(300, 599, *it,
		//src[0]), *(end - 1) ));

	//std::cout << BLUE << "size = " << nameVar->length() << std::endl;
	for (size_t i = 0; i < 3; i++)
		env.push_back(std::make_pair(nameVar[i], valueVar[i]));

	std::vector<std::pair<std::string, std::string>>::iterator it_env;

	for (it_env = env.begin(); it_env != env.end(); it_env++)
	{
		std::cout << BLUE << it_env->first;
		std::cout << END << " = " << RED << it_env->second << std::endl << END;
	}
	/* CONTENT_TYPE all types of var env possible
	CONTENT_LENGTH
	SCRIPT_FILENAME
	SCRIPT_NAME
	PATH_INFO
	PATH_TRANSLATED
	REQUEST_URI
	DOCUMENT_URI
	DOCUMENT_ROOT
	SERVER_PROTOCOL
	GATEWAY_INTERFACE
	SERVER_SOFTWARE
	REMOTE_ADDR
	REMOTE_PORT
	SERVER_ADDR
	SERVER_PORT
	SERVER_NAME */
}

std::vector<std::pair<std::string, std::string>> Cgi::getEnv() const
{
	return (this->env);
}

/* ************************************************************************** */
