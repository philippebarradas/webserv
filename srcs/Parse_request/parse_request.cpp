/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/02/14 15:45:40 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

/**
 * Initialise le comparateur
 **/

Parse_request::Parse_request() : _nbr_line(0)
{
	//std::cout << GREEN << "----------------- Start Parse Header -----------------" << END << std::endl << std::endl;
// GET /../../../Makefile HTTP/1.1 = invalid mais bon
    std::string  elements[] = {
		"Status", //ok
		"Method", //ok
		"Path", //ok
		"Query",
		"Protocol", //ok
		"Host:", //ok
		"Host-uncut-comme-les-casquettes",
		"A-IM:",
		"Transfer-Encoding:"
		"Accept:",
		"Accept-Charset:",
		"Accept-Encoding:",
		"Accept-Language:",
		"Accept-Datetime:",
		"Access-Control-Request-Method:",
		"Access-Control-Request-Headers:",
		"Authorization:",
		"Cache-Control:",
		"Connection:", //ok
		"Content-Length:", //ok
		"Content-Type:",
		"Cookie:",
		"Date:",
		"Expect:",
		"Forwarded:","From:",
		"If-Match:",
		"If-Modified-Since:", //
		"If-None-Match:",
		"If-Range:",
		"If-Unmodified-Since:", //
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
		"Last-Modified:",
		"Warning:"};

	std::string empty = "";
	size_t len = sizeof(elements) / sizeof(std::string);
	for (size_t x = 0; x < len ; x++)
		_big_tab.insert(std::pair<std::string, std::string>(elements[x], empty));
}

	_next_buffer_is_body = 0;
	std::cout << GREEN <<"_next_buffer_is_body " << _next_buffer_is_body << END << std::endl << std::endl;


		_client_max_body_size = 10;
	//if (_next_buffer_is_body != 1)
	//{
		for (size_t x = 0; x < 42; x++)
			_header_tab.insert(std::pair<std::string, std::string>(elements[x], empty));
	//}
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int		Parse_request::buff_is_valid(char *buff)
{
	std::cout << GREEN <<"inside _next_buffer_is_body " << _next_buffer_is_body << END << std::endl << std::endl;
  	if (_next_buffer_is_body == TRUE && _request_body_size == 0)
	{
		this->_buffer = buff;
		std::cout << GREEN << "_request_body_size=[" << _request_body_size << "]" << END << std::endl;
		for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
		{
			if (it->second.size() != 0)
				std::cout << RED << "[" << it->first << "] = [" << it->second << "]" << END << std::endl;
		}
		return (check_request());
		//_next_buffer_is_body = 0;
	}
	std::map<std::string, std::string>::iterator replace;
	size_t	start = 0;

	if (init_buffer(buff) == -1)
		return (KEEP);
	if (_buffer.size() > _client_max_body_size)
	{
		replace = _header_tab.find("Status");
		replace->second = "413";
		std::cout << "ERROR 413 STATUS" << std::endl;
		return (STOP);
	}
	//std::cout << "buff_is_valid == \n{"<< _buffer << "}" << std::endl;
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
			std::cout << "---seg---" << std::endl;

	parse_path();
	return (check_first_line(full_size));
}

void	Parse_request::parse_path()
{
	size_t to;
	size_t stop = 0;
	size_t start = 0;
	std::map<std::string, std::string>::iterator replace;
	std::string path_tmp = get_request("Path");

	if (get_request("Path").find("?") != std::string::npos)
	{
		start = get_request("Path").find("?");
		replace = _header_tab.find("Query");
		replace->second = get_request("Path").substr(start + 1, get_request("Path").size() - start);
		replace = _header_tab.find("Path");
		replace->second = get_request("Path").substr(0, start);
	}
	while ((start = path_tmp.find("/")) != std::string::npos && stop == 0)
	{
		if ((to = path_tmp.find("/", start + 1)) == std::string::npos)
		{
			full_path.push_back(path_tmp.substr(0, to));
			stop = 1;
		}
		else
		{
			full_path.push_back(path_tmp.substr(0, to));
			path_tmp = path_tmp.substr(to, path_tmp.size() - to);
		}
	}
}

int		Parse_request::fill_variables()
{
	std::string cmp;
	size_t	final_pose = 0;
	size_t	found = 0;
	bool	bn = false;

	std::map<std::string, std::string>::iterator replace;
	for (std::map<std::string, std::string>::iterator ith = _header_tab.begin() ; ith != _header_tab.end(); ++ith)
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
			replace = _header_tab.find(ith->first);
			if (check_double_content(replace) == -1)
				return (STOP);
			if (replace->first.find(":") != std::string::npos)
				replace->second = fill_header_tab(_buffer.substr(found + (ith->first).size(), final_pose - (found + (ith->first).size())));
		}
	}
	if (get_request("Expect:").compare("100-continue") == 0)
	{
		set_next_buffer_is_body(TRUE);
		std::cout << GREEN <<"FIND 100-continue  _next_buffer_is_body " << _next_buffer_is_body << END << std::endl << std::endl;
	}
	//DISPLAY VALID ELEMENTS
	for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
    {
		if (it->second.size() != 0)
			std::cout << "[" << it->first << "] = [" << it->second << "]" << std::endl;
	}
	//
	return (KEEP);
}

int		Parse_request::init_buffer(char *buff)
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
		return (STOP);
	for(x = 0; buff[x] == '\r' && buff[x + 1] == '\n' && x < std::strlen(buff); x += 2);
	this->_buffer = buff + x;
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

Parse_request&				Parse_request::operator=( Parse_request const & rhs )
{
	(void)rhs;
	return *this;
}
