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

Response::Response( Parse_request const & req, TreatRequest const & treat )
{
	this->writeRequestStatus(req.get_request("Status"));
	this->_header += "Server: webserv/1.0 (Ubuntu)\n";
	this->writeDate();
	this->writeType(treat.getExtension(), treat);
	this->writeLenght(treat.getFile());
	this->_header += "Connection: " + req.get_request("Connection:") + "\n";

	this->_header += "\n" + treat.getFile();
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

void	Response::writeRequestStatus( std::string const & code )
{
	std::string		all_code[] = {"200", "400", "403", "404", "405"};
	std::string		all_status[] = {"OK", "Bad Request", "Forbidden",
		"Not Found", "Not Allowed"};
	size_t			len, i;

	this->_header += "HTTP/1.1 " + code;

	len = sizeof(all_code) / sizeof(std::string);
	for (i = 0 ; i < len ; ++i)
	{
		if (all_code[i] == code)
		{
			this->_header += " " + all_status[i] + "\n";
			break;
		}
	}
	this->_header += " Not Define\n";
}

void	Response::writeType( std::string const & extension, TreatRequest const & treat )
{
	if (treat.getIs_Cgi())
		this->_header += treat.getType_Cgi() + "\n";
	else
	{
		this->_header += "Content-Type: ";
		if (extension == ".html")
			this->_header += "text/html";
		else
			this->_header += "text/plain";
		this->_header += "\n";
	}
}

void	Response::writeLenght( std::string const & page )
{
	std::stringstream conv;

	conv << page.length();
	this->_header += "Content-Length: " + conv.str() + "\n";
}

void	Response::writeDate( void )
{
	char		buffer[200];
	time_t		rawtime;
	struct tm	*timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 200, "%a, %d %b %G %T %Z",timeinfo);
	this->_header += "Date: " + std::string(buffer) + "\n";
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::string const &	Response::getHeader( void ) const
{
	//std::cout << "_header\t=\t\n" << _header << std::endl;
	return (this->_header);
}


/* ************************************************************************** */
