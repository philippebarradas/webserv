/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/01/14 15:04:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.hpp"
/* {	
	std::string listen;
	std::string eg;
	
	size_t size_start = 0;
	size_t size_end = 0;
	size_t nbr_dp = 0;
	size_t nbr_bn = 0;

	for ( std::string::iterator it = buff.begin(); it != buff.end(); ++it)
	{
		eg = *it;
		if (nbr_dp < 2)
			size_start++;
		if ((eg).compare(":") == 0)
			nbr_dp++;
		if ((eg).compare("\n") == 0)
			nbr_bn++;
		if (nbr_bn < 2)
			size_end++;
	}
	listen = buff.substr(size_start, size_end - size_start - 1);
	return (listen);
}
 */

int		Parse_header::parse_first_line(std::string buffer)
{
	std::string cmp;
	size_t start = 0;
	size_t size = 0;
	size_t full_size = 0;
	size_t rank = 0;

	for (std::string::iterator it = buffer.begin(); it != buffer.end() && rank <= 2; ++it)
	{
		cmp = *it;
		if (cmp.compare(" ") != 0 && cmp.compare("\n") != 0)
			size++;
		else if (cmp.compare(" ") == 0 || cmp.compare("\n") == 0)
		{
			if (rank == 0)
				this->_method = buffer.substr(start, size);
			else if (rank == 1)
				this->_path = buffer.substr(start, size);
			else if (rank == 2)
			{
				if (cmp.compare("\n") == 0)
					this->_protocol = buffer.substr(start, size - 1);
				else
					this->_protocol = buffer.substr(start, size);
			}
			full_size += size + 1;
			start = full_size;
			size = 0;
			rank++;
		}
	}
	std::cout << "~[" << get_method() << "]" << std::endl;
	std::cout << "~[" << get_path() << "]" << std::endl;
	std::cout << "~[" << get_protocol() << "]" << std::endl;
	if (get_method().compare("GET") != 0
	&& get_method().compare("POST") != 0
	&& get_method().compare("DELETE") != 0)
	{
		this->_request_status = 404; 
		return (1);
	}
	if (get_protocol().compare("HTTP/1.1") != 0)
	{
		this->_request_status = 400; 
		return (1);
	}
	return (0);
}

int		Parse_header::buff_is_valid(char *buff)
{
	std::string buffer = buff;
	std::string cmp;

	size_t start = 0;
	size_t size = 0;
	size_t full_size = 0;
	
	size_t rank = 0;

	this->incr_nbr_line();



	if (get_nbr_line() == 1)
	{
		if (parse_first_line(buffer) == 1)
			return (1);
	}
	
	for (std::string::iterator it = buffer.begin(); it != buffer.end(); ++it)
	{
		cmp = *it;
	
		if (cmp.compare("\n") == 0)
		{
			this->incr_nbr_line();
			std::cout << "nbr line = " << this->get_nbr_line() << std::endl;
		}
	}
	//std::cout << "len = " << buffer.size() << std::endl;

	if (buffer.size() > 10)
		return (1);
	else
		return (0);
	return (0);
}


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Parse_header::Parse_header()
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Parse_header::~Parse_header()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
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
