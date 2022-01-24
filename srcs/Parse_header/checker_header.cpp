/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_header.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 10:56:28 by user42            #+#    #+#             */
/*   Updated: 2022/01/24 19:11:37 by user42           ###   ########.fr       */
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

int		Parse_header::check_double_content(std::map<std::string, std::string>::iterator replace)
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
	pos = _buffer.find("If-Unmodified-Since\r\n");
	if (pos != std::string::npos)
	{
		pos = _buffer.find("If-Unmodified-Since\r\n", pos + 1);
		if (pos != std::string::npos)
		{
			std::cout << "ERROR DOUBLE un - modified since" << std::endl;
			replace = _big_tab.find("status");
			replace->second = "400";
			return (-1);
		}
	}

	pos = _buffer.find("If-Modified-Since\r\n");
	if (pos != std::string::npos)
	{
		pos = _buffer.find("If-Modified-Since\r\n", pos + 1);
		if (pos != std::string::npos)
		{
			std::cout << "ERROR DOUBLE modified since" << std::endl;
			replace = _big_tab.find("status");
			replace->second = "400";
			return (-1);
		}
	}
	return (0);
}

#include <sys/stat.h>


int		Parse_header::check_precondition()
{
    std::string time_test = get_request("If-Unmodified-Since:");
	
	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (-1);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (-1);
	if (time_test.compare("") == 0)
		return (0);
	if (time_test.size() < 13)
		return (-1);

    std::string filename = "srcs/Config/default/html_page/404_not_found.html";
    struct stat result;

   	if (stat(filename.c_str(), &result) == -1)
	{
	    std::cout << "stat failed" << std::endl;
		return (-1);
	}

	char time_modified_file [200];
	struct tm * timeinfo;
	
	timeinfo = localtime (&result.st_ctim.tv_sec);
	strftime(time_modified_file, 200, "%a, %d %b %G %T %Z", timeinfo);
	std::string actual_time(time_modified_file);

    struct tm timeinfo_modif;
    struct tm timeinfo_test;

    strptime(time_modified_file , "%a, %d %b %G %T %Z", &timeinfo_modif);
    strptime(time_test.c_str(), "%a, %d %b %G %T %Z", &timeinfo_test);


	if (timeinfo_modif.tm_year > timeinfo_test.tm_year)
		return (-1);
	else if (timeinfo_modif.tm_year < timeinfo_test.tm_year)
		return (0);
	else
	{
		if (timeinfo_modif.tm_yday > timeinfo_test.tm_yday)
			return (-1);
		else if (timeinfo_modif.tm_yday < timeinfo_test.tm_yday)
			return (0);
		else
		{
			if (timeinfo_modif.tm_hour > timeinfo_test.tm_hour)
				return (-1);
			else if (timeinfo_modif.tm_hour < timeinfo_test.tm_hour)
				return (0);
			else
			{
				if (timeinfo_modif.tm_min > timeinfo_test.tm_min)
					return (-1);
				else if (timeinfo_modif.tm_min < timeinfo_test.tm_min)
					return (0);
				else
				{
					if (timeinfo_modif.tm_sec > timeinfo_test.tm_sec)
						return (-1);
					else if (timeinfo_modif.tm_sec < timeinfo_test.tm_sec)
						return (0);
				}
			}
		}
	} 
	return (0);
}

int		Parse_header::check_header()
{
	std::map<std::string, std::string>::iterator replace;
	size_t	found = 0;

	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		if (get_request("Connection:").find("close") != std::string::npos)
		{
			replace = _big_tab.find("Connection:");
			replace->second = "close";
		}
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
