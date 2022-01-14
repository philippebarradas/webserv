/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/01/14 18:36:01 by user42           ###   ########.fr       */
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

	if ((get_method().compare("GET") != 0 && get_method().compare("POST") != 0
	&& get_method().compare("DELETE") != 0) || (get_path().at(0) != '/'))
	{
		std::cout << "ici 1" << std::endl;
		this->_request_status = 400; 
		return (-1);
	}
	else if (get_protocol().compare("HTTP/1.1") != 0)
	{
		std::cout << "ici 2" << std::endl;
		this->_request_status = 404; 
		return (-1);
	}
	return (full_size);
}

void	Parse_header::fill_elements(int pos, std::string str)
{
	if (pos == 0)
		_host = str;
	if (pos == 1)
		_user_agent = str;
	if (pos == 2)
		_accept = str;
	if (pos == 3)
		_accept_language = str;
	if (pos == 4)
		_accept_encoding = str;
	if (pos == 5)
		_method_charset = str;
	if (pos == 6)
		_keep_alive = str;
	if (pos == 7)
		_connection = str;

	std::cout << "_host = [" << _host << "]" << std::endl;
	std::cout << "_user_agent = [" << _user_agent << "]" << std::endl;
	std::cout << "_accept = [" << _accept << "]"<< std::endl;
	std::cout << "_accept_language = [" << _accept_language << "]"<< std::endl;
	std::cout << "_accept_encoding = [" << _accept_encoding << "]"<< "]"<< std::endl;
	std::cout << "_method_charset = [" << _method_charset << std::endl;
	std::cout << "_keep_alive = [" << _keep_alive << "]"<< std::endl;
	std::cout << "_connection = [" << _connection << "]"<< std::endl;
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

   //	all_header["Host:"].push_back("Host:");


   	all_header.push_back("Host:");
  	all_header.push_back("User_agent:");
  	all_header.push_back("Accept:");
  	all_header.push_back("Accept Language:");
  	all_header.push_back("Accept Encoding:");
  	all_header.push_back("Method Charset:");
  	all_header.push_back("Keep Alive:");
  	all_header.push_back("Connection:");

	if (get_nbr_line() == 1)
	{
		if ((start = parse_first_line(buffer)) == -1)
			return (-1);
		if (start >= buffer.size())
			return (0);
		else
			buffer = buffer.substr(start, buffer.size() - start);;
		//std::cout << "start = " << start << " -- [" << buffer.at(start - 1) << "]" <<  std::endl;
	}

	for (std::vector<std::string>::iterator ith = all_header.begin() ; ith != all_header.end(); ++ith)
	{
    	//std::cout << "string =|" << *ith << "|" << std::endl;
	}

	size_t found = 0;
	size_t final_pose = 0;

	size_t pos = 0;

	bool bn = false;
	std::cout << std::endl << "now = [" << buffer << "]" << std::endl; 
	//for (std::string::iterator it = buffer.begin(); it != buffer.end(); ++it)
	//{
		
		
/* 		cmp = *it;
		//found = buffer.find(*it)
		if (cmp.compare("\n") == 0)
		{
			this->incr_nbr_line();
			std::cout << "nbr line = " << this->get_nbr_line() << std::endl;
		} */

		for (std::vector<std::string>::iterator ith = all_header.begin() ; ith != all_header.end(); ++ith)
		{
			found = buffer.find(*ith);
			if (found!=std::string::npos)
			{
				final_pose = 0;
				bn = false;
				for (std::string::iterator it = buffer.begin(); it != buffer.end() && bn == false; ++it)
				{
					final_pose++;
					cmp = *it;
					if (final_pose > found && cmp.compare("\n") == 0)
						bn = true;
				}

				std::cout << "[" << *ith << "] at = [" << found << "]"
				<< " = ["
				<< buffer.substr(found + (*ith).size(), final_pose - (found + (*ith).size()))
				<< "]"
				<< " final_pose = [" << final_pose << "]"
				<< std::endl;

				fill_elements(pos, buffer.substr(found + (*ith).size(), final_pose - (found + (*ith).size())));
				//all_header["host:"] = buffer.substr(found + (*ith).size(), final_pose - (found + (*ith).size()));
			}
			pos++;
		}
	//}
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
