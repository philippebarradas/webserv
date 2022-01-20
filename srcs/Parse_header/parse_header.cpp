/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/01/20 08:22:50 by user42           ###   ########.fr       */
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

    std::string  elements[39] = {
		"status", //ok
		"method", //ok
		"path", //ok
		"protocol",//ok
		"Host:",//ok
		"A-IM:", 
		"Accept:",
		"Accept-Charset:",
		"Accept-Encoding:", 
		"Accept-Language:",
		"Accept-Datetime:", 
		"Access-Control-Request-Method:",
		"Access-Control-Request-Headers:", 
		"Authorization:",
		"Cache-Control:", 
		"Connection:",//ok
		"Content-Length:",//ok
		"Content-Type:",
		"Cookie:",
		"Date:",
		"Expect:",
		"Forwarded:","From:",
		"If-Match:",
		"If-Modified-Since:",
		"If-None-Match:",
		"If-Range:",
		"If-Unmodified-Since:", 
		"Max-Forwards:",
		"Origin:", 
		"Pragma:",
		"Proxy-Authorization:", 
		"Range:",
		"Referer:",
		"TE:",
		"User-Agent:",
		"Upgrade:", 
		"Via:", 
		"Warning:"};

	std::string empty = "";
	for (size_t x = 0; x < 39; x++)
		_big_tab.insert(std::pair<std::string, std::string>(elements[x], empty));
	//_big_tab.insert(std::pair<std::string, std::string>("Content-Length", "NULL"));
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
	std::map<std::string, std::string>::iterator replace;
	std::string buffer_line = line;
	size_t	start = 0;

	if (init_buffer(buff) == -1)
		return (0);
	if (_buffer.size() > 32000)
	{
		replace = _big_tab.find("status");
		replace->second = "413";
		return (-1);
	}
	std::cout << "buffer == \n{"<< _buffer << "}" << std::endl;
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
	if (fill_variables() == -1)
		return (-1);
	return (check_header());
}

int		Parse_header::init_buffer(char *buff)
{
	int		reset = 0;
	size_t	x = 0;

	for(size_t r = 0; r < std::strlen(buff); r += 2)
	{
		if (buff[r] != '\r' || buff[r + 1] != '\n')
			reset = 1;
		if (reset != 1 && buff[r] == '\r' && buff[r + 1] == '\n')
			reset = 2;
	}
	if (this->_nbr_line == 0 && reset == 2)
		return (-1);
	for(x = 0; buff[x] == '\r' && buff[x + 1] == '\n' && x < std::strlen(buff); x += 2);
	this->_buffer = buff + x;
	return (0);
}

int		Parse_header::parse_first_line()
{
	std::string cmp;
	size_t start = 0;
	size_t size = 0;
	size_t full_size = 0;
	size_t rank = 0;
	std::map<std::string, std::string>::iterator replace;

	for (std::string::iterator it = _buffer.begin(); it != _buffer.end() && rank <= 2; ++it)
	{
		cmp = *it;
		if (cmp.compare(" ") != 0 && cmp.compare("\n") != 0)
			size++;
		else if (cmp.compare(" ") == 0 || cmp.compare("\n") == 0)
		{
			if (rank == 0)
			{
				replace = _big_tab.find("method");
				replace->second = _buffer.substr(start, size);
			}
			else if (rank == 1)
			{
				replace = _big_tab.find("path");
				replace->second = _buffer.substr(start, size);
			}
			else if (rank == 2)
			{
				replace = _big_tab.find("protocol");
				if (cmp.compare("\n") == 0)
					replace->second = _buffer.substr(start, size - 1);
				else
					replace->second =  _buffer.substr(start, size);
			}
			full_size += size + 1;
			start = full_size;
			size = 0;
			rank++;
		}
	}
	return (check_first_line(full_size));
}

int		Parse_header::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	bool	bn = false;

	std::map<std::string, std::string>::iterator replace;
	for (std::map<std::string, std::string>::iterator ith = _big_tab.begin() ; ith != _big_tab.end(); ++ith)
	{
		found = _buffer.rfind(ith->first);
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
			replace = _big_tab.find(ith->first);
			if (check_double_content_length(replace) == -1)
				return (-1);
			if (replace->first.find(":") != std::string::npos)
				replace->second = fill_big_tab(_buffer.substr(found + (ith->first).size(), final_pose - (found + (ith->first).size())));
		}
	}
 
	//DISPLAY VALID ELEMENTS
	for (std::map<std::string, std::string>::iterator it = _big_tab.begin(); it != _big_tab.end(); ++it)
    {
		if (it->second.size() != 0)
			std::cout << "[" << it->first << "] = [" << it->second << "]" << std::endl;
	}
	return (0);
	//
}

std::string	Parse_header::fill_big_tab(std::string str)
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