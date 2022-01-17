/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/01/17 13:56:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

/**
 * Initialise le comparateur
 **/
 
Parse_header::Parse_header() : _nbr_line(0)
{
	std::cout << GREEN << "----------------- Start Parse Header -----------------" << END << std::endl << std::endl;

   	all_header.push_back("Host:");
  	all_header.push_back("User_agent:");
  	all_header.push_back("Accept:");
  	all_header.push_back("Accept Language:");
  	all_header.push_back("Accept Encoding:");
  	all_header.push_back("Method Charset:");
  	all_header.push_back("Keep Alive:");
  	all_header.push_back("Connection:");
	all_header.push_back("Content Type:");
  	all_header.push_back("Content Length:");
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Parse_header::~Parse_header()
{
	std::cout << GREEN << "----------------- End Parse Header -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Parse_header&				Parse_header::operator=( Parse_header const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int		Parse_header::buff_is_valid(char *buff, char *line)
{
	this->_buffer = buff;
	std::string buffer_line = line;
	size_t	found = 0;
	size_t start = 0;
	
	this->incr_nbr_line();
	if (get_nbr_line() == 1)
	{
		if ((start = parse_first_line()) == -1)
			return (-1);
		if (start >= _buffer.size())
			return (0);
		else
			_buffer = _buffer.substr(start, _buffer.size() - start);;
	}

	if (buffer_line.compare("\r\n") == 0)
	{
		if (get_host().size() == 0)
			_request_status = 400;
		std::cout << " == \\n" << std::endl;
		return (1);
	}
	fill_variables();

	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
		return (1);
	return (0);
}

int		Parse_header::parse_first_line()
{
	std::string cmp;
	size_t start = 0;
	size_t size = 0;
	size_t full_size = 0;
	size_t rank = 0;

	for (std::string::iterator it = _buffer.begin(); it != _buffer.end() && rank <= 2; ++it)
	{
		cmp = *it;
		if (cmp.compare(" ") != 0 && cmp.compare("\n") != 0)
			size++;
		else if (cmp.compare(" ") == 0 || cmp.compare("\n") == 0)
		{
			if (rank == 0)
				this->_method = _buffer.substr(start, size);
			else if (rank == 1)
				this->_path = _buffer.substr(start, size);
			else if (rank == 2)
			{
				if (cmp.compare("\n") == 0)
					this->_protocol = _buffer.substr(start, size - 1);
				else
					this->_protocol = _buffer.substr(start, size);
			}
			full_size += size + 1;
			start = full_size;
			size = 0;
			rank++;
		}
	}

	if ((get_method().compare("GET") != 0 && get_method().compare("POST") != 0
	&& get_method().compare("DELETE") != 0) || (get_path().at(0) != '/'))
	{
		std::cout << "ici 1" << std::endl;
		this->_request_status = 400; 
		return (-1);
	}
	else if (get_protocol().compare("HTTP/1.1") != 0)
	{
		std::cout << "ici 2" << std::endl;
		this->_request_status = 404; 
		return (-1);
	}
	else
		this->_request_status = 200;
	return (full_size);
}

void		Parse_header::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	size_t	pos = 0;
	bool	bn = false;

	for (std::vector<std::string>::iterator ith = all_header.begin() ; ith != all_header.end(); ++ith)
	{
		found = _buffer.find(*ith);
		if (found != std::string::npos)
		{
			final_pose = 0;
			bn = false;
			for (std::string::iterator it = _buffer.begin(); it != _buffer.end() && bn == false; ++it)
			{
				final_pose++;
				cmp = *it;
				if (final_pose > found && cmp.compare("\n") == 0)
					bn = true;
			}
			fill_elements(pos, _buffer.substr(found + (*ith).size(), final_pose - (found + (*ith).size())));
		}
		pos++;
	}
}

void	Parse_header::fill_elements(int pos, std::string str)
{
	if (!str.empty() && str[str.size() - 1] == '\n')
		str.erase(str.size() - 1);
	if (!str.empty() && str[str.size() - 1] == '\r')
		str.erase(str.size() - 1);
	while (!str.empty() && str[str.size() - 1] == ' ')
		str.erase(str.size() - 1);
	while (!str.empty() && str[0] == ' ')
		str.erase(0,1);

	if (pos == 0)
		_host = str;
	else if (pos == 1)
		_user_agent = str;
	else if (pos == 2)
		_accept = str;
	else if (pos == 3)
		_accept_language = str;
	else if (pos == 4)
		_accept_encoding = str;
	else if (pos == 5)
		_method_charset = str;
	else if (pos == 6)
		_keep_alive = str;
	else if (pos == 7)
		_connection = str;
	else if (pos == 8)
		_content_type = str;
	else if (pos == 9)
		_content_length = str;
}
