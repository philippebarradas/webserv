/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 07:35:32 by user42            #+#    #+#             */
/*   Updated: 2022/03/01 13:17:47 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Parse_request::Parse_request()
{
	size_t		len;
	std::string	elements[] = {
		"Status",
		"Method",
		"Path",
		"Query",
		"Protocol",
		"Host-uncut",
	};
	_nbr_line = 0;
	_request_body = "";
	std::string empty = "";
	_request_body_size = 0;
	first_line_is_parsed = false;
	error_first_line = false;

	len = sizeof(elements) / sizeof(std::string);
	for (size_t x = 0; x < len; x++)
		_header_tab.insert(std::pair<std::string,
			std::string>(elements[x], empty));
	_header_tab["Status"] = "200";
}

Parse_request::~Parse_request()
{
}

Parse_request::Parse_request( Parse_request const & src )
{
	*this = src;
}

Parse_request&	Parse_request::operator=( Parse_request const & rhs )
{
	this->error_first_line = rhs.error_first_line;
	this->first_line_is_parsed = rhs.first_line_is_parsed;
	this->_buffer = rhs._buffer;
	this->_request_body_size = rhs._request_body_size;
	this->_request_body = rhs._request_body;
	this->_header_tab = rhs._header_tab;
	this->_nbr_line = rhs._nbr_line;
	this->_param_request_tab = rhs._param_request_tab;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int		Parse_request::parse_request(std::string full_buffer)
{
	size_t	start = 0;

 	this->_buffer = full_buffer;
	this->incr_nbr_line();
	if (get_nbr_line() == 1)
	{
		if ((start = fill_first_line()) == -1)
			return (-1);
		if (start >= _buffer.size())
			return (0);
	}
	if (fill_variables() == -1)
		return (-1);
	return (check_request());
}

bool			str_is_lnt(std::string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isalpha(*it) && !std::isdigit(*it) && *it != '-')
			return (false);
	}
	return (true);
}

void		Parse_request::fill_param_request_tab(std::string buff_parsed)
{
	size_t	final_pose = 0, end = 0;
	size_t debut = 0;
	size_t	found = 0;

	while ((found = buff_parsed.find("\r\n")) != std::string::npos)
	{
		end = buff_parsed.find("\r\n\r\n");
		if ((debut = buff_parsed.substr(0, found).find(":")) != std::string::npos)
		{
			if ((final_pose = buff_parsed.find("\r\n")) != std::string::npos
				&& str_is_lnt(buff_parsed.substr(0, debut)))
			{
				_param_request_tab.insert(std::pair<std::string, std::string>
					(buff_parsed.substr(0, debut)
					,fill_header_tab(buff_parsed.substr(debut + 1
						,final_pose - debut - 1))));
			}
		}
		else if (str_is_lnt(buff_parsed.substr(0, found)) == 0
			&& buff_parsed.substr(0, found).size() != 0)
			_param_request_tab.insert(std::pair<std::string, std::string>
				(buff_parsed.substr(0, found), ""));
		buff_parsed = buff_parsed.substr(found + 2, buff_parsed.size() - (final_pose));
		if (found == end)
			break;
	}
}

int		Parse_request::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	bool	bn = false;
	std::string buff_parsed = _buffer;

	if ((found =_buffer.find("\r\n")) != std::string::npos)
		buff_parsed = buff_parsed.substr(found + 2, buff_parsed.size() - (found + 2));

	fill_param_request_tab(buff_parsed);
	while ((found = buff_parsed.find(":")) != std::string::npos)
	{
		found += 1;
		for (std::string::iterator it = buff_parsed.begin();
			it != buff_parsed.end() && bn == false; ++it)
		{
			final_pose++;
			cmp = *it;
			if (final_pose > found && cmp.compare("\n") == 0)
				bn = true;
		}
		if (bn == true)
		{
			_header_tab.insert(std::pair<std::string, std::string>
			(buff_parsed.substr(0, found)
			,fill_header_tab(buff_parsed.substr(found, final_pose - found))));
			bn = false;
		}
		buff_parsed = buff_parsed.substr(final_pose, buff_parsed.size() - (final_pose));
		final_pose = 0;
	}
	return (0);
}


std::string	Parse_request::fill_header_tab(std::string str)
{
	if (!str.empty() && str[str.size() - 1] == '\n')
		str.erase(str.size() - 1);
	if (!str.empty() && str[str.size() - 1] == '\r')
		str.erase(str.size() - 1);
	while (!str.empty() && str[str.size() - 1] == ' ')
		str.erase(str.size() - 1);
	while (!str.empty() && str[0] == ' ')
		str.erase(0,1);
	return (str);
}

// GET

std::string Parse_request::get_request(std::string request) const
{
	for (std::map<std::string, std::string>::const_iterator
		it = _header_tab.begin(); it != _header_tab.end(); ++it)
	{
		if (request.compare(it->first) == 0)
		{
			if (it->second.compare("") != 0)
				return (it->second);
			return ("");
		}
	}
	return ("");
}

size_t	Parse_request::get_nbr_line() const
{
	return this->_nbr_line;
}

std::map<std::string, std::string>	Parse_request::getBigMegaSuperTab(void) const
{
	return this->_header_tab;
}

std::map<std::string, std::string>	const & Parse_request::get_param_request_tab( void ) const
{
	return this->_param_request_tab;
}

std::string Parse_request::get_request_body() const
{
	return (_request_body);
}

size_t Parse_request::get_request_body_size() const
{
	return (_request_body_size);
}

// SET

void	Parse_request::setStatus( std::string const & code )
{
	this->_header_tab["Status"] = code;
}

void	Parse_request::setConnection( std::string const & status)
{
	this->_header_tab["Connection:"] = status;
}

void	Parse_request::setTransfer( std::string const & method )
{
	this->_header_tab["Transfer-Encoding:"] = method;
}

void 	Parse_request::set_request_body(std::string new_request_body)
{
	_request_body = new_request_body;
}

void	Parse_request::incr_nbr_line()
{
	this->_nbr_line++;
}
