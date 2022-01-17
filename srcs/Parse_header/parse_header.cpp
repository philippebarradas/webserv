/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:25:34 by user42            #+#    #+#             */
/*   Updated: 2022/01/17 08:28:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.hpp"

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
	else
		this->_request_status = 200;
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
}


int		Parse_header::buff_is_valid(char *buff, char *line)
{
	std::string buffer = buff;
	std::string buffer_line = line;

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
	std::cout << std::endl << "buffer = [" << buffer << "]" << std::endl; 
	std::cout << std::endl << "line = [" << line << "]" << std::endl; 

/* 	for (int x = 0; line[x]; x++)
	{
		printf("line[x] = %d\n", line[x]);
	} */

	if (buffer_line.compare("\r\n") == 0)
	{
		if (get_host().size() == 0)
			_request_status = 400;
		std::cout << " == \\n" << std::endl;
		return (1);
	}

	for (std::vector<std::string>::iterator ith = all_header.begin() ; ith != all_header.end(); ++ith)
	{
		found = buffer.find(*ith);
		if (found != std::string::npos)
		{
			final_pose = 0;
			bn = false;
			for (std::string::iterator it = buffer.begin(); it != buffer.end() && bn == false; ++it)
			{
				final_pose++;
				cmp = *it;
				if (final_pose > found && cmp.compare("\r") == 0)
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
	std::cout << "x_requesr_status = [" << get_request_status() << "]" << std::endl;
	found = buffer.find("\n\r\n");
	if (found != std::string::npos)
	{
		std::cout << " a trouve l'orange du marchand " << std::endl;
		return (1);
	}
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
