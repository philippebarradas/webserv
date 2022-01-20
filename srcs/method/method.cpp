
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:58:14 by user42            #+#    #+#             */
/*   Updated: 2022/01/11 15:49:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method.hpp"
#include <iostream>
#include <cstring>
#define TRUE 1
#define FALSE 0
#include <vector>


std::string Method::build_header(std::string buff)
{
	std::string full_header;
	//std::cout << "buff = [" << buff << "]" << std::endl;
	buff = "wait";
  	//for (int i=1; i<=5; i++)
  	_header.push_back(_request_status);
  	_header.push_back(_server);
	_header.push_back(_date);
  	_header.push_back(_content_type);
  	_header.push_back(_content_length);
  	_header.push_back(_connection);


  //	std::cout << "vector contains:" << std::endl;
  	for (std::vector<std::string>::iterator it = _header.begin() ; it != _header.end(); ++it)
	{
		full_header += *it;
		full_header += "\n";
    //	std::cout << "string =|" << *it << std::endl;
	}
	full_header += "\n";
	
	//std::cout << "full = [" << full_header << "]" << std::endl;
	return (full_header);
	//return (bad_request);
}


std::string	get_actual_listen(std::string buff)
{	
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

size_t get_listen_vector(std::vector<Server> src, std::string act_listen)
{
	size_t f = 0;

	for(src.begin(); f < src.size() &&
	src[f].getListen().compare(0 , act_listen.size(), act_listen) != 0 ; ++f)
	{
		//	std::cout << "get [" << src[f].getListen() << "]" << std::endl;
		//	std::cout << "act [" << act_listen << "]" << std::endl;

	}
	return (f);
}

std::string	int_to_string(int x);

std::string Method::is_method(std::string buff, std::vector<Server> src, int port, const Parse_header & parse_head) // true or false
{

	std::string act_listen = int_to_string(port);//get_actual_listen(buff);

	//std::cout << "listen = " << act_listen << std::endl << std::endl;

	size_t j = get_listen_vector(src, act_listen);

	std::cout << "xstatus = " << parse_head.get_request("status") << std::endl;
	//std::cout << "j = " << j << " size = " << src.size() << " size listen = " << act_listen.size() << std::endl;
	if (parse_head.get_request("status").compare("400") == 0)//j == src.size() || act_listen.size() == 0)
	{
		std::cout << "x bad request" << std::endl;
		return (is_bad_request(buff));
	}
	else if (parse_head.get_request("status").compare("404") == 0)//j == src.size() || act_listen.size() == 0)
	{
		std::cout << "x not found" << std::endl;
		return (is_not_found(buff));
	}
	else if (parse_head.get_request("status").compare("405") == 0)//j == src.size() || act_listen.size() == 0)
	{
		std::cout << "x not allowed" << std::endl;
		return (is_not_allowed(buff));
	}
	else if (parse_head.get_request("status").compare("412") == 0)//j == src.size() || act_listen.size() == 0)
	{
		std::cout << "x precondition failed" << std::endl;
		return (is_precondition_failed(buff));
	}
	else if (parse_head.get_request("status").compare("413") == 0)//j == src.size() || act_listen.size() == 0)
	{
		std::cout << "x header or cookie too large" << std::endl;
		return (is_too_large(buff));
	}
	//std::cout << src[j] << std::endl;

	std::set<std::string>				_methods;
	std::map<std::string, Server>		_location;
	_methods = src[j].getMethods();
	_location = src[j].getLocation();

	//std::cout << "ici" << std::endl;



	std::map<std::string, Server>::iterator it_location = _location.begin();
	for(it_location = _location.begin(); it_location != _location.end(); ++it_location)
	{
		//std::cout << "-->" << (it_location->first) << "<--" << std::endl;
	}
	//std::cout << _methods << std::endl;
 	std::set<std::string>::iterator it_method;
	std::string act_method = parse_head.get_request("method");

	std::cout << "act_method = |" << act_method << "|" << std::endl;

	bool i = false;
	
	for (it_method = _methods.begin() ; it_method != _methods.end(); it_method++)
	{
		if ((*it_method).compare(0 , (*it_method).size(), act_method) == 0)
			i = true;
		//std::cout << (*it_method) << std::endl;
	}
	if (i == false)
	{
		std::cout << "false method" << std::endl;
		return (is_not_allowed(buff));
	}
	
	std::set<std::string>				_index = src[j].getIndex();
	std::set<std::string>::iterator		it_index = _index.begin();

 	for (it_index = _index.begin() ; it_index != _index.end(); it_index++)
	{	this->act_index = (*it_index);} 

	//std::cout << "file = " << src[j].getIndex() << std::endl;
	if (act_method.compare("GET") == 0)
		return ft_get(buff, parse_head);
	else if (act_method.compare("POST") == 0)
		return ft_post(buff, parse_head);
	else if (act_method.compare("DELETE") == 0)
		return ft_delete(buff, parse_head);

	//return (ft_get(buff));
	
	//return (is_bad_request(buff));

	//return ("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
	//std::cout << "\n\n [method GET] \n\n" << std::endl;

	return ("HTTP/1.1 200 Good Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>200 good Request</title></head>\n<body bgcolor='white'>\n<center><h1>200 good Request</h1></center>\n<hr><center>webcerveau/1.0 (Ubuntu)</center>\n</body>\n</html>\0");
}

Method::Method(void)
{
}

Method::~Method(void)
{
}

Method::Method(Method const & Method)
{
	*this = Method;
}

Method	&Method::operator=(const Method &pt)
{
	this->_request_status = pt._request_status;
	return (*this);
}
