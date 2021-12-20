#include "method.hpp"
#include <iostream>
#include <cstring>
#define TRUE 1
#define FALSE 0
#include <vector>

/* int	Method::is_method(std::string name_cmd, std::string full_cmd) // true or false
{
	int new_len = full_cmd.length() - 2;
	std::string new_full_cmd = full_cmd.substr(0, new_len);
	if (!name_cmd.compare(new_full_cmd))
		return (TRUE);
	return (FALSE); */

std::string Method::is_bad_request(std::string buff)
{
	std::string bad_request;

	return ("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
	return (bad_request);
}

std::string Method::is_method(std::string buff) // true or false
{
	if (buff.compare("GET ") == 0)
		return ft_get(buff);
	else if (buff.compare("POST ") == 0)
		return ft_post(buff);
	else if (buff.compare("DELETE ") == 0)
		return ft_delete(buff);
	//return (ft_get(buff));
	
	return (is_bad_request(buff));

	//return ("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
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
	return (*this);
}
