#include "parse_request.hpp"

size_t	hexa_to_size(std::string nbr)
{
	std::stringstream ss;
	std::string hex = "0123456789abcdefABCDEF";
	size_t found;
	size_t res = 0;
	size_t p = 0;

	for (std::string::iterator it = nbr.begin(); it != nbr.end(); ++it)
	{
		if ((found = hex.find(*it)) == std::string::npos)
			return (0);
	}
	ss << std::hex << nbr;
	ss >> res;
	return (res);
}

int			Parse_request::parse_body(std::string full_buffer)
{
	//std::cout << RED << "_next_buffer_is_body = ["<< _next_buffer_is_body << "]" << END << std::endl;
    //if (_next_buffer_is_body == true && _request_body_size == 0)
//	{
		this->_buffer = full_buffer;
		//std::cout << RED << "body ==  = ["<< full_buffer  << "]" << END << std::endl;
		return (check_request());
	//}
	//return (0);
}

void	Parse_request::is_body(size_t found)
{
	size_t line_size = 0;
	size_t size = -1;
	std::string _request_body_unchanked;
	std::string cmp = "\r\n";

	if (found != 0)
		found += 4;
	if (_buffer.size() > found)
		_request_body = _buffer.substr(found, _buffer.size() - found);
	if (_request_body.size() == 0)
		return ;
	std::string split_body = _request_body;


		//std::cout << RED << "request_body = ["<< _request_body << "]" << END << std::endl;
	if (get_request("Transfer-Encoding:") == "chunked")
	{
		//std::cout << RED << "CHUNKED ~~~~~~~~~~~~~ " << END << std::endl;
		while (((found = split_body.find("\r\n")) != std::string::npos) && (size != 0))
		{
			found += cmp.size();
			size = hexa_to_size(split_body.substr(0, found - cmp.size()));
			if (size != 0)
				_request_body_size += size;
			else
				_request_body_unchanked += split_body.substr(0, found - cmp.size());
			split_body = split_body.substr(found, split_body.size() - found);
		}
		_request_body = _request_body_unchanked;
	}
	if (_request_body_size == 0)
		_request_body_size = _request_body.size();
	else if (_request_body_size == 0 && get_request("Content-Length:").compare("") != 0)
		_request_body_size = std::stoi(get_request("Content-Length:"));
	//if (_next_buffer_is_body == 1 && _request_body_size != 0)
	//	_next_buffer_is_body = 0;
}
