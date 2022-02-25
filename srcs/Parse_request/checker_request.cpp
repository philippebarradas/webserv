/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_request.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 10:56:28 by user42            #+#    #+#             */
/*   Updated: 2022/02/25 19:09:36 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

int		Parse_request::check_path()
{
	if (get_request("Path") == "")
		return (1);
	else if (get_request("Path").at(0) != '/')
		return (1);
	if (get_request("Path").find("../") != std::string::npos)
		return (1);
	return (0);
}

int     Parse_request::check_first_line(size_t full_size)
{
	std::map<std::string, std::string>::iterator replace;
	//std::cout << RED << "check_path()=[" << check_path() << "]" << END << std::endl;

    replace = _header_tab.find("Status");
	if ((get_request("Method").compare("GET") != 0 && get_request("Method").compare("POST") != 0
	&& get_request("Method").compare("DELETE") != 0) || (check_path() != 0))
	{
		replace->second = "400";
		std::cout << "request_status = " << _header_tab["Status"] << std::endl;
		std::cout << "{ERROR 400}" << std::endl;
		return (STOP);
	}
	else if (get_request("Protocol").compare("HTTP/1.1") != 0)
	{
		if (get_request("Protocol").find("HTTP/") != std::string::npos)
			replace->second = "505";
		else
			replace->second = "404";
		std::cout << "request_status = " << _header_tab["Status"] << std::endl;
		return (STOP);
	}
	else
		replace->second = "200";
	return (full_size);
}

int		Parse_request::check_double_content()
{
	size_t	pos = 0;

	std::map<std::string, std::string>::iterator replace;
	if (get_request("Content-Length:") != "")
	{
		replace = _header_tab.find("Content-Length:");
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
					replace = _header_tab.find("Status");
					replace->second = "400";
					return (STOP);
				}
			}
		}
	}
	if ((pos = _buffer.find("If-Unmodified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Unmodified-Since\r\n", pos + 1)) != std::string::npos)
		{
			//std::cout << "ERROR DOUBLE un - modified since" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
			return (STOP);
		}
	}
	if ((pos = _buffer.find("If-Modified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Modified-Since\r\n", pos + 1)) != std::string::npos)
		{
			//std::cout << "ERROR DOUBLE modified since" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
			return (STOP);
		}
	}
	return (KEEP);
}

int		Parse_request::check_precondition()
{
    std::string time_test = get_request("If-Unmodified-Since:");
	std::cout <<  time_test << std::endl;
	if (time_test.compare("") == 0)
		return (KEEP);
 	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (STOP);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (STOP);
	return (KEEP);
}

int		Parse_request::check_request()
{
	std::map<std::string, std::string>::iterator replace;
	size_t	found = 0;

	/* for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
    {
		std::cout << CYAN << "[" << it->first << "] = [" << it->second << "]" << END << std::endl;
	} */

	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		is_body(found);
		if (get_request("Connection:").find("close") != std::string::npos)
		{
			replace = _header_tab.find("Connection:");
			replace->second = "close";
		}
		else
		{
			replace = _header_tab.find("Connection:");
			replace->second = "keep-alive";
		}
		if (get_request("Host:").compare("") == 0)
		{
            //std::cout << "ERROR = NO HOST" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
		}
		else if (get_request("Host:").find(":") != std::string::npos)
		{
			found = get_request("Host:").find(":");

			replace = _header_tab.find("Host-uncut-comme-les-casquettes");
			replace->second = get_request("Host:").substr(0, found);

			replace = _header_tab.find("Host:");
			replace->second = get_request("Host:").substr(found + 1, get_request("Host:").size() - found);
		}
		if ((get_request("Content-Length:").compare("") != 0 && get_request("Content-Length:").find_first_not_of("0123456789") != std::string::npos)
		|| (_buffer.rfind("Content-Length\r\n") != std::string::npos))
		{
			//std::cout << "ERROR = BAD CONTENT LENGTH" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
		}
		else if (check_precondition() == -1)
		{
			//std::cout << "ERROR = Precondition Failed" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "412";
		}
		return (1);
	}
	std::cout << "CRASSSHHHH" << std::endl;
	return (KEEP);
}
