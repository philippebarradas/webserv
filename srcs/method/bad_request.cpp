/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bad_request.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:57:30 by user42            #+#    #+#             */
/*   Updated: 2022/01/11 15:59:08 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method.hpp"
#include <ctime>

std::string file_to_string(std::string file_path, std::string buff)
{
    std::ifstream ifs;
	std::string	line, file;

	char pwd[100];

	getcwd(pwd, 100);

	std::cout << pwd << std::endl;
	std::cout << file_path << std::endl;

    ifs.open(file_path, std::ifstream::in);
	if (!(ifs.is_open()))
		return ("not found");
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}
	ifs.close();
    return (file);
}
//#include <time>
std::string	Method::get_date()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [200];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime(buffer, 200, "Date: %a, %d %b %G %T %Z",timeinfo);
	std::string actual_time(buffer);
	
	return (actual_time);
}


std::string Method::is_not_found(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/404_not_found.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 400 Bad Request";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + std::to_string(file.size());
    _connection = "Connection: keep-alive";


	return (build_header(buff) + file);
}

std::string Method::is_bad_request(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/400_bad_request.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 400 Bad Request";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + std::to_string(file.size());
    _connection = "Connection: keep-alive";


	return (build_header(buff) + file);
}



std::string Method::is_not_allowed(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/405_not_allowed.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 405 Not Allowed";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + std::to_string(file.size());
    _connection = "Connection: keep-alive";

	//char cwd[100];
	//getcwd(cwd, 100);
	//std::cout << cwd << std::endl;



	//std::cout << "||||||||->" << file  << "<-||||||||" << std::endl;

	return (build_header(buff) + file);
}