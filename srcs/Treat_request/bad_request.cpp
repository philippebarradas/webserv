/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bad_request.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:57:30 by user42            #+#    #+#             */
/*   Updated: 2022/01/20 14:56:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "treat_request.hpp"
#include <ctime>

std::string file_to_string(std::string file_path, std::string buff)
{
    std::ifstream ifs;
	std::string	line, file;
	buff = "sal";
	char pwd[100];

	getcwd(pwd, 100);

	std::cout << pwd << std::endl;
	std::cout << file_path << std::endl;

    ifs.open(file_path.c_str(), std::ifstream::in);
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
std::string	Treat_request::get_date()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [200];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime(buffer, 200, "%a, %d %b %G %T %Z",timeinfo);
	std::string actual_time(buffer);
	
	return (actual_time);
}

std::string Treat_request::is_not_found(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/404_not_found.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 400 Bad Request";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_length = "Content-Length: " + int_to_string(file.size());
    _content_type = "Content-Type: text/html";
    _connection = "Connection: keep-alive";


	return (build_header(buff) + file);
}

std::string Treat_request::is_bad_request(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/400_bad_request.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 400 Bad Request";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size());
    _connection = "Connection: keep-alive";


	return (build_header(buff) + file);
}



std::string Treat_request::is_not_allowed(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/405_not_allowed.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 405 Not Allowed";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size());
    _connection = "Connection: keep-alive";

	//char cwd[100];
	//getcwd(cwd, 100);
	//std::cout << cwd << std::endl;



	//std::cout << "||||||||->" << file  << "<-||||||||" << std::endl;

	return (build_header(buff) + file);
}

std::string Treat_request::is_too_large(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/413_too_large.html", buff);
	//std::cout << "seg est ici [BAD REQUEST]" << std::endl;
    _request_status = "HTTP/1.1 400 Bad Request";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size());
    _connection = "Connection: keep-alive";

	//char cwd[100];
	//getcwd(cwd, 100);
	//std::cout << cwd << std::endl;



	//std::cout << "||||||||->" << file  << "<-||||||||" << std::endl;

	return (build_header(buff) + file);
}

std::string Treat_request::is_precondition_failed(std::string buff)
{
    std::string	file = file_to_string("srcs/Config/default/html_page/412_precondition_failed.html", buff);

    _request_status = "HTTP/1.1 412 precondition_failed";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size());
    _connection = "Connection: keep-alive";

	//char cwd[100];
	//getcwd(cwd, 100);
	//std::cout << cwd << std::endl;



	//std::cout << "||||||||->" << file  << "<-||||||||" << std::endl;

	return (build_header(buff) + file);
}