/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:09 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/07 19:02:53 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
}

Server::Server( std::string const & src )
{
	size_t	i;
	std::vector< std::vector<std::string> >	toParce;
	std::string	strParce;

	for (i = 0 ; isspace(src[i]) && src[i] ; ++i) {}
	if (src[i] != '{')
	{
		std::string thr("[Error] invalid characters after server bloc '");
		for (; !isspace(src[i]) && src[i] != '{' && src[i]; ++i)
			thr += src[i];
		thr += "'.";
		throw std::invalid_argument(thr);
	}


	strParce = std::string(&src[i + 1]);
	toParce = sortInVec(strParce);

	//this->parsing_all(toParce);
}

Server::Server( Server const & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

/*std::string	nextConf( std::string const & src , size_t & i )
{
	std::string	ret;

	for (; isspace(src[i]) && src[i] ; ++i) {}
	for (i ; !isspace(src[i]) && src[i] ; ++i)
		ret += src[i];

	return (ret);
}*/

void	Server::setName( std::string const & src )
{
	size_t	i = 0, e;

	while (src[i])
	{
		for (; isspace(src[i]) && src[i] ; ++i) {}
		for (e = i ; !isspace(src[e]) && src[e] ; ++e) {}
		if (i != e)
			this->server_name.insert(std::string(src.begin() + i,
			src.begin() + e));
		i = e;
	}
}

typedef void ( Server::*allFunction )( std::string const & );

void	Server::parsingAll( std::string & src )
{
	std::string	nameAllowed[]= {"server_name"};
	allFunction	setFunct[] = {&Server::setName};
	std::string	nameConf;
	size_t		i = 0;

	while (src[i])
	{
		//warn check if location has 2 bracket
		//nameConf = nextWord( src, i );
		std::cout << "nameConf = " << nameConf << std::endl;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
