/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Answer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:40:33 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/11 09:19:32 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Answer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Answer::Answer( unsigned int const & code, std::string const & page )
{
	setRequestStatus(code);
	this->_server_name = "webserv/0.1 (Ubuntu)";
	setDate();

}

Answer::Answer()
{
}

Answer::Answer( Answer const & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Answer::~Answer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Answer &				Answer::operator=( Answer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Answer const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Answer::writeRequestStatus( unsigned int const & code )
{
	unsigned int	all_code[] = {200, 404};
	std::string		all_status[] = {"OK", "Not Found"};
	stringstream	conv;
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

void	Answer::writeDate( void )
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


std::string	Answer::getHeader( void ) const
{
	return (this->_header);
}


/* ************************************************************************** */
