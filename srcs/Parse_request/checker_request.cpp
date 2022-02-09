/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_request.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 10:56:28 by user42            #+#    #+#             */
/*   Updated: 2022/02/09 08:15:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_request.hpp"

int     Parse_request::check_first_line(size_t full_size)
{
	std::map<std::string, std::string>::iterator replace;

    replace = _header_tab.find("Status");
	if ((get_request("Method").compare("GET") != 0 && get_request("Method").compare("POST") != 0
	&& get_request("Method").compare("DELETE") != 0) || (get_request("Path").at(0) != '/'))
	{
		replace->second = "400";
		std::cout << "request_status = " << _header_tab["Status"] << std::endl;
		return (STOP);
	}
	else if (get_request("Protocol").compare("HTTP/1.1") != 0)
	{
		replace->second = "404";
		std::cout << "request_status = " << _header_tab["Status"] << std::endl;
		return (STOP);
	}
	else
		replace->second = "200";
	return (full_size);
}

int		Parse_request::check_double_content(std::map<std::string, std::string>::iterator replace)
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
				replace = _header_tab.find("Status");
				replace->second = "400";
				return (STOP);
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
			replace = _header_tab.find("Status");
			replace->second = "400";
			return (STOP);
		}
	}
	pos = _buffer.find("If-Modified-Since\r\n");
	if (pos != std::string::npos)
	{
		pos = _buffer.find("If-Modified-Since\r\n", pos + 1);
		if (pos != std::string::npos)
		{
			std::cout << "ERROR DOUBLE modified since" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
			return (STOP);
		}
	}
	return (KEEP);
}

#include <sys/stat.h>
int		Parse_request::check_precondition()
{
    std::string time_test = get_request("If-Unmodified-Since:");

	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (STOP);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (STOP);
	if (time_test.compare("") == 0)
		return (KEEP);
	if (time_test.size() < 13)
		return (STOP);

    std::string filename = "srcs/Config/default/html_page/404_not_found.html";
    struct stat result;

   	if (stat(filename.c_str(), &result) == -1)
	{
	    std::cout << "stat failed" << std::endl;
		return (STOP);
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
		return (STOP);
	else if (timeinfo_modif.tm_year < timeinfo_test.tm_year)
		return (KEEP);
	else
	{
		if (timeinfo_modif.tm_yday > timeinfo_test.tm_yday)
			return (STOP);
		else if (timeinfo_modif.tm_yday < timeinfo_test.tm_yday)
			return (KEEP);
		else
		{
			if (timeinfo_modif.tm_hour > timeinfo_test.tm_hour)
				return (STOP);
			else if (timeinfo_modif.tm_hour < timeinfo_test.tm_hour)
				return (KEEP);
			else
			{
				if (timeinfo_modif.tm_min > timeinfo_test.tm_min)
					return (STOP);
				else if (timeinfo_modif.tm_min < timeinfo_test.tm_min)
					return (KEEP);
				else
				{
					if (timeinfo_modif.tm_sec > timeinfo_test.tm_sec)
						return (STOP);
					else if (timeinfo_modif.tm_sec < timeinfo_test.tm_sec)
						return (KEEP);
				}
			}
		}
	}
	return (KEEP);
}

int		Parse_request::check_request()
{
	std::map<std::string, std::string>::iterator replace;
	size_t	found = 0;

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
            std::cout << "ERROR = NO HOST" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
		}
		else if (get_request("Host:").find(":") != std::string::npos)
		{
			found = get_request("Host:").find(":");
			replace = _header_tab.find("Host:");
			replace->second = get_request("Host:").substr(found + 1, get_request("Host:").size() - found);
		}
		if ((get_request("Content-Length:").compare("") != 0 && get_request("Content-Length:").find_first_not_of("0123456789") != std::string::npos)
		|| (_buffer.rfind("Content-Length\r\n") != std::string::npos))
		{
			std::cout << "ERROR = BAD CONTENT LENGTH" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "400";
		}
		else if (check_precondition() == -1)
		{
			std::cout << "ERROR = Precondition Failed" << std::endl;
			replace = _header_tab.find("Status");
			replace->second = "412";
		}
		return (1);

	}
	return (KEEP);
}
