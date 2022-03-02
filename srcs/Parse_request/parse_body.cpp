/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_body.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 07:33:11 by user42            #+#    #+#             */
/*   Updated: 2022/03/02 12:26:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

size_t	hexa_to_size(std::string nbr)
{
	std::stringstream ss;
	std::string hex = "0123456789abcdefABCDEF";
	size_t found;
	size_t res = 0;

	for (std::string::iterator it = nbr.begin(); it != nbr.end(); ++it)
	{
		if ((found = hex.find(*it)) == std::string::npos)
			return (0);
	}
	ss << std::hex << nbr;
	ss >> res;
	return (res);
}

int			Parse_request::parse_body(std::string full_buffer)
{
	this->_buffer = full_buffer;
	return (check_request());
}

void	Parse_request::is_body(size_t found)
{
	size_t size = -1;
	std::string _request_body_unchanked;
	std::string cmp = "\r\n";

	if (found != 0)
		found += 4;
	if (_buffer.size() > found)
		_request_body = _buffer.substr(found, _buffer.size() - found);
	if (_request_body.size() == 0)
		return ;
	std::string split_body = _request_body;

	if (get_request("Transfer-Encoding:") == "chunked")
	{
		while (((found = split_body.find("\r\n")) != std::string::npos) && (size != 0))
		{
			found += cmp.size();
			size = hexa_to_size(split_body.substr(0, found - cmp.size()));
			if (size != 0)
				_request_body_size += size;
			else
				_request_body_unchanked += split_body.substr(0, found - cmp.size());
			split_body = split_body.substr(found, split_body.size() - found);
		}
		_request_body = _request_body_unchanked;
	}
	if (_request_body_size == 0)
		_request_body_size = _request_body.size();
	else if (_request_body_size == 0 && get_request("Content-Length:").compare("") != 0)
		_request_body_size = stost_size(0, MAX_MAXBODY,
			get_request("Content-Length:"), "_request_body_size");
}