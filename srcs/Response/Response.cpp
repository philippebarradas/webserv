/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:40:33 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/16 09:39:48 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response( Parse_request const & req, TreatRequest const & treat )
{
	this->writeRequestStatus(req.get_request("Status"));
	this->_header += "Server: webserv/1.0 (Ubuntu)\r\n";
	this->writeDate();
	this->writeType(treat.getExtension(), treat);
	this->writeLenght(treat.getFile());
	this->_header += treat.getLocation()[0]
		? "Location: " + treat.getLocation() + "\r\n"
		: "";
	this->_header += treat.getLastModif()[0]
		? "Last-Modified: " + treat.getLastModif() + "\r\n"
		: "";
	this->_header += "Connection: " + req.get_request("Connection:") + "\r\n";

	this->_header += "\r\n" + treat.getFile();
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
	std::string		all_code[] = {"200", "301", "400", "403", "404", "405",
		"412", "413"};
	std::string		all_status[] = {"OK", "Moved Permanently", "Bad Request", "Forbidden",
		"Not Found", "Not Allowed", "Precondition Failed", "Request Entity Too Large"};
	size_t			len, i;

	this->_header += "HTTP/1.1 " + code;

	len = sizeof(all_code) / sizeof(std::string);
	for (i = 0 ; i < len ; ++i)
	{
		if (all_code[i] == code)
		{
			this->_header += " " + all_status[i] + "\r\n";
			return ;
		}
	}
	this->_header += " Not Define\r\n";
}

void	Response::writeType( std::string const & extension, TreatRequest const & treat )
{
	if (treat.getIs_Cgi())
		this->_header += treat.getType_Cgi() + "\r\n";
	else
	{
		this->_header += "Content-Type: ";
		if (extension == ".html")
			this->_header += "text/html";
		else
			this->_header += "text/plain";
		this->_header += "\r\n";
	}
}

void	Response::writeLenght( std::string const & page )
{
	std::stringstream conv;

	conv << page.length();
	this->_header += "Content-Length: " + conv.str() + "\r\n";
}

void	Response::writeDate( void )
{
	char		buffer[200];
	time_t		rawtime;
	struct tm	*timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 200, "%a, %d %b %G %T %Z",timeinfo);
	this->_header += "Date: " + std::string(buffer) + "\r\n";
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::string const &	Response::getHeader( void ) const
{
	//std::cout << "_header\t=\t\r\n" << _header << std::endl;
	return (this->_header);
}


/* ************************************************************************** */
