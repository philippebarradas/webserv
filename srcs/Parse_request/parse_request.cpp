/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/02/23 11:09:38 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Parse_request::Parse_request()
{
	//std::cout << GREEN << "----------------- Start Parse Header -----------------" << END << std::endl << std::endl;
// GET /../../../Makefile HTTP/1.1 = invalid mais bon
	std::string  elements[] = {
		"Status", //ok
		"Method", //ok
		"Path", //ok
		"Query", //ok
		"Protocol", //ok
		"Host-uncut-comme-les-casquettes",
		"Connection:"
	};


	std::string empty = "";
	_next_buffer_is_body = 0;
	_request_body_size = 0;
	_nbr_line = 0;
	_request_body = "";

	for (size_t x = 0; x < 5; x++)
		_header_tab.insert(std::pair<std::string, std::string>(elements[x], empty));

	//for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
   // {
	//	std::cout << CYAN << "[" << it->first << "] = [" << it->second << "]" << END << std::endl;
	//}
}

Parse_request&				Parse_request::operator=( Parse_request const & rhs )
{
	this->_buffer = rhs._buffer;
	this->_next_buffer_is_body = rhs._next_buffer_is_body;
	this->_request_body_size = rhs._request_body_size;
	this->_request_body = rhs._request_body;
	this->_header_tab = rhs._header_tab;
	this->_nbr_line = rhs._nbr_line;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int		Parse_request::parse_request_buffer(std::string full_buffer)
{
	std::map<std::string, std::string>::iterator replace;
	size_t	start = 0;

	if (_next_buffer_is_body == TRUE && _request_body_size == 0)
	{
		this->_buffer = full_buffer;
		return (check_request());
	}


	//std::cout << PURPLE << "element=[" << get_nbr_line() << "]" << END << std::endl;
	this->_buffer = full_buffer;
	//std::cout << PURPLE << "full_buffer=[" << full_buffer << "]" << END << std::endl;
	//std::cout << PURPLE << "buffer=[" << this->_buffer << "]" << END << std::endl;

	this->incr_nbr_line();
	if (get_nbr_line() == 1)
	{
		if ((start = parse_first_line()) == -1)
			return (STOP);
		if (start >= _buffer.size())
			return (KEEP);
		else
			_buffer = _buffer.substr(start, _buffer.size() - start);;
	}
	if (fill_variables() == -1)
		return (STOP);
	return (check_request());
}

int		Parse_request::parse_first_line()
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
		if (cmp.compare(" ") != 0 && cmp.compare("\n") != 0 && it != _buffer.end())
			size++;
		else if ((cmp.compare(" ") == 0 || cmp.compare("\n") == 0) && it != _buffer.end())
		{
			if (rank == 0)
			{
				replace = _header_tab.find("Method");
				replace->second = _buffer.substr(start, size);
			}
			else if (rank == 1)
			{
				replace = _header_tab.find("Path");
				replace->second = _buffer.substr(start, size);
			}
			else if (rank == 2)
			{
				replace = _header_tab.find("Protocol");
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

	parse_path();
	return (check_first_line(full_size));
}

void	Parse_request::parse_path()
{
	size_t to;
	size_t stop = 0;
	size_t start = 0;
	std::map<std::string, std::string>::iterator replace;

	if (get_request("Path").find("?") != std::string::npos)
	{
		start = get_request("Path").find("?");
		replace = _header_tab.find("Query");
		replace->second = get_request("Path").substr(start + 1, get_request("Path").size() - start);
		replace = _header_tab.find("Path");
		replace->second = get_request("Path").substr(0, start);
	}
}

int			str_is_lnt(std::string str)
{
	std::string accept = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPKRSTUVWXYZ-0123456789";
	bool v = false;

	//std::cout << BLUE << "str=[" << str << "]" << END << std::endl;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		v = false;
		for (std::string::iterator ita = accept.begin(); ita != accept.end(); ++ita)
		{
			if (*it == *ita)
				v = true;
		}
		if (v == false)
			return (1);
	}
	return (0);
}

void		Parse_request::fill_param_request_tab()
{
	size_t	final_pose = 0;
	size_t debut = 0;
	size_t	found = 0;
	std::string buff_parsed = _buffer;
	size_t x = 0;
	std::map<std::string, std::string>::iterator replace;

	while ((found = buff_parsed.find("\r\n")) != std::string::npos)
	{
		if ((debut = buff_parsed.substr(0, found).find(":")) != std::string::npos)
		{
			if ((final_pose = buff_parsed.find("\r\n")) != std::string::npos)
			{
				_param_request_tab.insert(std::pair<std::string, std::string>
				(buff_parsed.substr(0, debut)
				,fill_header_tab(buff_parsed.substr(debut + 1, final_pose - debut - 1))));
			}
		}
		else if (x != 0 && str_is_lnt(buff_parsed.substr(0, found)) == 0)
			_param_request_tab.insert(std::pair<std::string, std::string>(buff_parsed.substr(0, found), ""));//fill_header_tab(buff_parsed.substr(found, final_pose - found))));
		buff_parsed = buff_parsed.substr(found + 2, buff_parsed.size() - (final_pose));
		//final_pose = 0;
		x++;
	}
/* 	for (std::map<std::string, std::string>::iterator it = _param_request_tab.begin(); it != _param_request_tab.end(); ++it)
    {
		//if (it->second.size() != 0)
		std::cout << YELLOW << "[" << it->first << "] = [" << it->second << "]" <<  END << std::endl;
	} */
}

int		Parse_request::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	bool	bn = false;
	std::string buff_parsed = _buffer;
	std::map<std::string, std::string>::iterator replace;

	fill_param_request_tab();

	while ((found = buff_parsed.find(":")) != std::string::npos)
	{
		found += 1;
		for (std::string::iterator it = buff_parsed.begin(); it != buff_parsed.end() && bn == false; ++it)
		{
			final_pose++;
			cmp = *it;
			if (final_pose > found && cmp.compare("\n") == 0)
				bn = true;
		}
		if (bn == true)
		{
			//if (check_double_content(_header_tab.find(buff_parsed.substr(0, found))) == -1)
			//	return (STOP);
			_header_tab.insert(std::pair<std::string, std::string>
			(buff_parsed.substr(0, found)
			,fill_header_tab(buff_parsed.substr(found, final_pose - found))));
			bn = false;
		}
		buff_parsed = buff_parsed.substr(final_pose, buff_parsed.size() - (final_pose));
		final_pose = 0;
	}
	if (get_request("Expect:") == "100-continue")
	{
		set_next_buffer_is_body(TRUE);
		std::cout << GREEN << "FIND 100-continue  _next_buffer_is_body " << _next_buffer_is_body << END << std::endl << std::endl;
	}
	//DISPLAY VALID ELEMENTS
/* 	for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
    {
		if (it->second.size() != 0)
			std::cout << "[" << it->first << "] = [" << it->second << "]" << std::endl;
	}
	//
	//std::cout << RED << "_=[" << _buffer<< "]" << END << std::endl;
	return (KEEP);
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

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Parse_request::~Parse_request()
{
	//std::cout << GREEN << "----------------- End Parse Header -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
