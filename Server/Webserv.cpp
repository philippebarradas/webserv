/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 18:58:44 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/02 19:32:25 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Webserv::Webserv()
{
	int		listen	= -1;
	int		one		= 1;
	this->fd_socket	= -1;

	//throw std::runtime_error("[Error] socket failed" + std::string(strerror(errno)));

	listen = socket(AF_INET, SOCK_STREAM, 0);
	if (listen < 0)
		throw std::runtime_error("[Error] socket failed" + std::string(strerror(errno)));

	this->fd_socket = setsockopt(listen, SOL_SOCKET,  SO_REUSEADDR,
		&one, sizeof(int));
	if (this->fd_socket < 0)
		throw std::runtime_error("[Error] setsockopt failed");

	this->fd_socket = fcntl(listen, F_SETFL, O_NONBLOCK);
}

Webserv::Webserv( const Webserv & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Webserv::~Webserv()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Webserv &				Webserv::operator=( Webserv const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Webserv const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
