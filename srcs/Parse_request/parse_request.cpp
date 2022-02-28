/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 07:35:32 by user42            #+#    #+#             */
/*   Updated: 2022/02/28 08:15:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Parse_request::Parse_request()
{
	//std::cout << GREEN << "----------------- Start Parse Header -----------------" << END << std::endl << std::endl;
	size_t		len;
	std::string	elements[] = {
		"Status", //ok
		"Method", //ok
		"Path", //ok
		"Query", //ok
		"Protocol",
		"Host-uncut-comme-les-casquettes",
		"Connection:"
	};

	_nbr_line = 0;
	_request_body = "";
	std::string empty = "";
	//_next_buffer_is_body = 0;
	_request_body_size = 0;
	first_line_is_parsed = false;
	error_first_line = false;


	len = sizeof(elements) / sizeof(std::string);
	for (size_t x = 0; x < len; x++)
		_header_tab.insert(std::pair<std::string, std::string>(elements[x], empty));
	_header_tab["Status"] = "200";
}

Parse_request::~Parse_request()
{
	//std::cout << GREEN << "----------------- End Parse Header -----------------" << END << std::endl << std::endl;
}

Parse_request&				Parse_request::operator=( Parse_request const & rhs )
{
	this->_buffer = rhs._buffer;
	//this->_next_buffer_is_body = rhs._next_buffer_is_body;
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

/* 	if (_next_buffer_is_body == TRUE && _request_body_size == 0)
	{
		this->_buffer = full_buffer;
		return (check_request());
	} */

 	this->_buffer = full_buffer;
	//std::cout << _nbr_line << std::endl;

	this->incr_nbr_line();
	//std::cout << _nbr_line << std::endl;

	if (get_nbr_line() == 1)
	{
		std::cout << "{fill first line}" << std::endl;
		if ((start = fill_first_line()) == -1)
			return (STOP);
		if (start >= _buffer.size())
			return (KEEP);
		//else
			//_buffer = _buffer.substr(start, _buffer.size() - start);;
	}
	if (fill_variables() == -1)
		return (STOP);
	return (check_request());
}

bool			str_is_lnt(std::string str)
{
	//std::cout << BLUE << "str=[" << str << "]" << END << std::endl;
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
			if ((final_pose = buff_parsed.find("\r\n")) != std::string::npos && str_is_lnt(buff_parsed.substr(0, debut)))
			{
				_param_request_tab.insert(std::pair<std::string, std::string>
					(buff_parsed.substr(0, debut)
					,fill_header_tab(buff_parsed.substr(debut + 1, final_pose - debut - 1))));
			}
		}
		else if (str_is_lnt(buff_parsed.substr(0, found)) == 0 && buff_parsed.substr(0, found).size() != 0)
			_param_request_tab.insert(std::pair<std::string, std::string>(buff_parsed.substr(0, found), ""));//fill_header_tab(buff_parsed.substr(found, final_pose - found))));
		buff_parsed = buff_parsed.substr(found + 2, buff_parsed.size() - (final_pose));
		if (found == end)
			break;
	}

 	/*for (std::map<std::string, std::string>::iterator it = _param_request_tab.begin(); it != _param_request_tab.end(); ++it)
	{
		//if (it->second.size() != 0)
		std::cout << YELLOW << "[" << it->first << "] = [" << it->second << "]" <<  END << std::endl;
	}*/
	//std::cout << "ENDDDDDDDDDDDDDDDdd" << std::endl;
}

int		Parse_request::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	bool	bn = false;
	std::string buff_parsed = _buffer;

	//std::cout << GREEN << "buff_parsed = ["<< buff_parsed << "]" << END << std::endl;
	if ((found =_buffer.find("\r\n")) != std::string::npos)
		buff_parsed = buff_parsed.substr(found + 2, buff_parsed.size() - (found + 2));
	//std::cout << GREEN << "buff_parsed = ["<< buff_parsed << "]" << END << std::endl;


	fill_param_request_tab(buff_parsed);
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
			_header_tab.insert(std::pair<std::string, std::string>
			(buff_parsed.substr(0, found)
			,fill_header_tab(buff_parsed.substr(found, final_pose - found))));
			bn = false;
		}
		buff_parsed = buff_parsed.substr(final_pose, buff_parsed.size() - (final_pose));
		final_pose = 0;
	}
	if (get_request("Connection:") == "")
		_header_tab["Connection:"] = "close";
	
	
	//if (get_request("Expect:") == "100-continue" || get_request("Content-Length:") != "")
	//{
		//set_next_buffer_is_body(TRUE);
	//	std::cout << GREEN << "FIND 100-continue  _next_buffer_is_body " << _next_buffer_is_body << END << std::endl << std::endl;
	//}
/*
	for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
    {
		if (it->second.size() != 0)
			std::cout << "[" << it->first << "] = [" << it->second << "]" << std::endl;
	}
*/
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
