/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_first_line.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 07:35:17 by user42            #+#    #+#             */
/*   Updated: 2022/02/28 11:28:58 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

int		Parse_request::parse_first_line(std::string full_buffer)
{
	size_t	start = 0;
	_buffer = full_buffer;

	this->incr_nbr_line();
	if (get_nbr_line() == 1)
	{
		if ((start = fill_first_line()) == -1)
			return (-1);
		if (start >= _buffer.size())
			return (0);
	}
	return (0);
}

int		Parse_request::fill_first_line()
{
	std::string cmp;
	size_t start = 0;
	size_t size = 0;
	size_t full_size = 0;
	size_t rank = 0;

	for (std::string::iterator it = _buffer.begin(); it != _buffer.end()
		&& rank <= 2; ++it)
	{
		cmp = *it;
		if (cmp.compare(" ") != 0 && cmp.compare("\n") != 0
			&& it != _buffer.end())
			size++;
		else if ((cmp.compare(" ") == 0 || cmp.compare("\n") == 0)
			&& it != _buffer.end())
		{
			if (rank == 0)
				_header_tab["Method"] = _buffer.substr(start, size);
			else if (rank == 1)
				_header_tab["Path"] = _buffer.substr(start, size);
			else if (rank == 2)
			{
				if (cmp.compare("\n") == 0)
					_header_tab["Protocol"] = _buffer.substr(start, size - 1);
				else
					_header_tab["Protocol"] = _buffer.substr(start, size);
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
	size_t start = 0;

	if (get_request("Path").find("?") != std::string::npos)
	{
		start = get_request("Path").find("?");
		_header_tab["Query"] = get_request("Path").substr(start + 1,
			get_request("Path").size() - start);
		_header_tab["Path"] = get_request("Path").substr(0, start);
	}
}

/* ************************ */
/*    CHECKER FIST LINE     */
/* ************************ */

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
	if ((get_request("Method").compare("GET") != 0
		&& get_request("Method").compare("POST") != 0
		&& get_request("Method").compare("DELETE") != 0) || (check_path() != 0))
	{
		_header_tab["Status"] = "400";
		return (-1);
	}
	else if (get_request("Protocol").compare("HTTP/1.1") != 0)
	{
		if (get_request("Protocol").find("HTTP/") != std::string::npos)
			_header_tab["Status"] = "505";
		else
			_header_tab["Status"] = "404";
		return (-1);
	}
	else
		_header_tab["Status"] = "200";
	return (full_size);
}
