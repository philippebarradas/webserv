#include "method.hpp"
#include <iostream>
#include <cstring>
#define TRUE 1
#define FALSE 0
#include <vector>

int	Method::is_method(std::string name_cmd, std::string full_cmd) // true or false
{
	int new_len = full_cmd.length() - 2;
	std::string new_full_cmd = full_cmd.substr(0, new_len);
	if (!name_cmd.compare(new_full_cmd))
		return (TRUE);
	return (FALSE);
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
