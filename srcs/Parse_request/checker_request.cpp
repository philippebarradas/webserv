/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_request.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 07:32:26 by user42            #+#    #+#             */
/*   Updated: 2022/03/01 14:28:15 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

int		Parse_request::check_double_content()
{
	size_t	pos = 0;

	std::map<std::string, std::string>::iterator replace;
	if (get_request("Content-Length:") != "")
	{
		replace = _header_tab.find("Content-Length:");
		if (replace->first.compare("Content-Length:") == 0
			&& replace->second.compare("") != 0)
		{
			pos = _buffer.find(replace->first);
			if (pos != std::string::npos)
			{
				pos += replace->first.size();
				pos = _buffer.find(replace->first, pos);
				if (pos != std::string::npos)
				{
					_header_tab["Status"] = "400";
					return (-1);
				}
			}
		}
	}
	if ((pos = _buffer.find("If-Unmodified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Unmodified-Since\r\n", pos + 1))
			!= std::string::npos)
		{
			_header_tab["Status"] = "400";
			return (-1);
		}
	}
	if ((pos = _buffer.find("If-Modified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Modified-Since\r\n", pos + 1))
			!= std::string::npos)
		{
			_header_tab["Status"] = "400";
			return (-1);
		}
	}
	return (0);
}

int		Parse_request::check_precondition()
{
    std::string time_test = get_request("If-Unmodified-Since:");
	if (time_test.compare("") == 0)
		return (0);
 	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (-1);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (-1);
	return (0);
}

int		Parse_request::check_request()
{
	size_t	found = 0;

	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		is_body(found);
		if (get_request("Connection:").find("close") != std::string::npos)
			_header_tab["Connection:"] = "close";
		else
			_header_tab["Connection:"] = "keep-alive";
		if (get_request("Host:").compare("") == 0)
			_header_tab["Status"] = "400";
		else if (get_request("Host:").find(":") != std::string::npos)
		{
			found = get_request("Host:").find(":");
			_header_tab["Host-uncut"] = get_request("Host:").substr(0, found);
			_header_tab["Host:"] = get_request("Host:").substr(found + 1,
				get_request("Host:").size() - found);
		}
		if ((get_request("Content-Length:").compare("") != 0
			&& get_request("Content-Length:").find_first_not_of("0123456789")
			!= std::string::npos)
			|| (_buffer.rfind("Content-Length\r\n") != std::string::npos))
			_header_tab["Status"] = "400";
		else if (check_precondition() == -1)
			_header_tab["Status"] = "412";
		return (1);
	}
	return (0);
}
