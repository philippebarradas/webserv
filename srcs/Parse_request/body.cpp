/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:58:10 by user42            #+#    #+#             */
/*   Updated: 2022/02/07 13:58:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

size_t	hexa_to_size(std::string nbr)
{
	std::stringstream ss;
	std::string hex = "0123456789abcdefABCDEF";
	size_t found;
	size_t res = 0;
	size_t p = 0;

	for (std::string::iterator it = nbr.begin(); it != nbr.end(); ++it)
	{
		if ((found = hex.find(*it)) == std::string::npos)
			return (-1);
	}
	
	ss << std::hex << nbr;
	ss >> res;
	
	std::cout << "nbr = [" << nbr << "]" << std::endl;
	std::cout << "res = " << res << std::endl;
	return (res);
}

void	Parse_request::is_body(size_t found)
{
	size_t line_size = 0;
	size_t pos = 0;
	size_t size = -1;
	std::string _request_body_unchanked;
	std::string cmp = "\r\n";

	if (_buffer.size() > found + 4)
		_request_body = _buffer.substr(found + 4, _buffer.size() - found + 4);
	if (_request_body.size() == 0)
		return ;

	std::string split_body = _request_body;

	//std::cout << "transfet = " << get_request("Transfer-Encoding:") << std::endl;

	if (get_request("Transfer-Encoding:").compare("chunked") == 0)
	{
		std::cout << "---------START----------" << std::endl;
		while (((found = split_body.find("\r\n")) != std::string::npos) && (size != 0))
		{
			found += cmp.size();
			size = hexa_to_size(split_body.substr(0, found - cmp.size()));
			if (size != -1)
				_request_body_size += size;
			else
				_request_body_unchanked += split_body.substr(0, found - cmp.size());
			//std::cout << "-------------------" << std::endl;
			//line_size = stoi(split_body.substr(0, found));
			//std::cout << "found = " << found << std::endl;
			//std::cout << "line = [" << split_body.substr(0, found - cmp.size()) << "]" << std::endl;
			
			//pos += found + 2;m
			
			split_body = split_body.substr(found, split_body.size() - found);
			//std::cout << "split_body = [" << split_body << "]" << std::endl;
		}

		std::cout << "_request_body_size = [" << _request_body_size << "]" << std::endl;
		//std::cout << "_request_body_unchanked = [" << _request_body_unchanked << "]" << std::endl;

		_request_body = _request_body_unchanked;

		std::cout << "--------END-----------" << std::endl;
	}
	//std::cout << "is_body = {{{{{" << _request_body << "}}}}}}}" << std::endl;
	if (_request_body_size == 0)
		_request_body_size = _request_body.size();
	std::cout << "_request_body_size = [" << _request_body_size << "]" << std::endl;

	if (_next_buffer_is_body == 1 && _request_body_size != 0)
		_next_buffer_is_body = 0;
}