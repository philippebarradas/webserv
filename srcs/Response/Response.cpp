/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:40:33 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/11 09:19:32 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response( unsigned int const & code, std::string const & page )
{
	this->writeRequestStatus(code);
	this->_header += "webserv/1.0 (Ubuntu)";
	this->writeDate();

	this->_header += "\n" + page;
}

Response::Response()
{
}

Response::Response( Response const & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Response &				Response::operator=( Response const & rhs )
{
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Response const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Response::writeRequestStatus( unsigned int const & code )
{
	unsigned int	all_code[] = {200, 404};
	std::string		all_status[] = {"OK", "Not Found"};
	std::stringstream	conv;
	size_t			len, i;

	conv << code;
	this->_header += "HTTP/1.1 " + conv.str();

	len = sizeof(all_code) / sizeof(unsigned int);

	for (i = 0 ; i < len ; ++i)
	{
		if (all_code[i] == code)
		{

		}
	}
}

void	Response::writeDate( void )
{
	char		buffer[200];
	time_t		rawtime;
	struct tm	*timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 200, "%a, %d %b %G %T %Z",timeinfo);
	this->_header += std::string(buffer);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::string	Response::getHeader( void ) const
{
	return (this->_header);
}


/* ************************************************************************** */
