/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_header.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 10:56:28 by user42            #+#    #+#             */
/*   Updated: 2022/01/20 08:58:21 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.hpp"

int     Parse_header::check_first_line(size_t full_size)
{
    std::map<std::string, std::string>::iterator replace;

    replace = _big_tab.find("status");
	if ((get_request("method").compare("GET") != 0 && get_request("method").compare("POST") != 0
	&& get_request("method").compare("DELETE") != 0) || (get_request("path").at(0) != '/'))
	{
		replace->second = "400";
		std::cout << "request_status = " << _big_tab["status"] << std::endl;
		return (-1);
	}
	else if (get_request("protocol").compare("HTTP/1.1") != 0)
	{
		replace->second = "404";
		std::cout << "request_status = " << _big_tab["status"] << std::endl;
		return (-1);
	}
	else
		replace->second = "200";
	return (full_size);
}

int		Parse_header::check_double_content_length(std::map<std::string, std::string>::iterator replace)
{
	size_t	pos = 0;

	if (replace->first.compare("Content-Length:") == 0 && replace->second.compare("") != 0)
	{
		pos = _buffer.find(replace->first);
		if (pos != std::string::npos)
		{
			pos += replace->first.size();
			pos = _buffer.find(replace->first, pos);
			if (pos != std::string::npos)
			{
				std::cout << "ERROR DOUBLE CONTENT LENGTH" << std::endl;
				replace = _big_tab.find("status");
				replace->second = "400";
				return (-1);
			}
		}
	}
	return (0);
}

int		Parse_header::check_precondition()
{
	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (-1);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (-1);
	return (0);
}

int		Parse_header::check_header()
{
	std::map<std::string, std::string>::iterator replace;
	size_t	found = 0;

	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
	{

		if (get_request("Host:").compare("") == 0)
		{
            std::cout << "ERROR = NO HOST" << std::endl;
			replace = _big_tab.find("status");
			replace->second = "400";
		}
		else if ((get_request("Content-Length:").compare("") != 0 && get_request("Content-Length:").find_first_not_of("0123456789") != std::string::npos)
		|| (_buffer.rfind("Content-Length\r\n") != std::string::npos))
		{
			std::cout << "ERROR = BAD CONTENT LENGTH" << std::endl;
			replace = _big_tab.find("status");
			replace->second = "400";
		}
		else if (check_precondition() == -1)
		{
			std::cout << "ERROR = Precondition Failed" << std::endl;
			replace = _big_tab.find("status");
			replace->second = "412";
		}
		return (1);

	}
	return (0); 
}