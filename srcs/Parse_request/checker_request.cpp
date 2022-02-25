#include "parse_request.hpp"

int		Parse_request::check_double_content()
{
	size_t	pos = 0;

	std::map<std::string, std::string>::iterator replace;
	if (get_request("Content-Length:") != "")
	{
		replace = _header_tab.find("Content-Length:");
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
					_header_tab["Status"] = "400";
					//replace = _header_tab.find("Status");
					//replace->second = "400";
					return (STOP);
				}
			}
		}
	}
	if ((pos = _buffer.find("If-Unmodified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Unmodified-Since\r\n", pos + 1)) != std::string::npos)
		{
			std::cout << "ERROR DOUBLE un - modified since" << std::endl;
			_header_tab["Status"] = "400";
			//replace = _header_tab.find("Status");
			//replace->second = "400";
			return (STOP);
		}
	}
	if ((pos = _buffer.find("If-Modified-Since\r\n")) != std::string::npos)
	{
		if ((pos = _buffer.find("If-Modified-Since\r\n", pos + 1)) != std::string::npos)
		{
			//std::cout << "ERROR DOUBLE modified since" << std::endl;
			_header_tab["Status"] = "400";
			//replace = _header_tab.find("Status");
			//replace->second = "400";
			return (STOP);
		}
	}
	return (KEEP);
}

int		Parse_request::check_precondition()
{
    std::string time_test = get_request("If-Unmodified-Since:");
	std::cout <<  time_test << std::endl;
	if (time_test.compare("") == 0)
		return (KEEP);
 	if(_buffer.rfind("If-Match\r\n") != std::string::npos)
		return (STOP);
	if(_buffer.rfind("If-Unmodified-Since\r\n") != std::string::npos)
		return (STOP);
	return (KEEP);
}

int		Parse_request::check_request()
{
	size_t	found = 0;
	
/*  std::cout << "CHECKER REQUEST" << std::endl;
	
 	for (std::map<std::string, std::string>::iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
	{
		std::cout << CYAN << "[" << it->first << "] = [" << it->second << "]" << END << std::endl;
	}
	 */
	found = _buffer.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		is_body(found);
		if (get_request("Connection:").find("close") != std::string::npos)
		{
			_header_tab["Connection:"] = "close";
			//replace = _header_tab.find("Connection:");
			//replace->second = "close";
		}
		else
		{
			_header_tab["Connection:"] = "keep-alive";
			//replace->second = "keep-alive";
		}
		if (get_request("Host:").compare("") == 0)
		{
            std::cout << "ERROR = NO HOST" << std::endl;
			_header_tab["Status"] = "400";
			//replace = _header_tab.find("Status");
			//replace->second = "400";
		}
		else if (get_request("Host:").find(":") != std::string::npos)
		{
			found = get_request("Host:").find(":");

			_header_tab["Host-uncut-comme-les-casquettes"] = get_request("Host:").substr(0, found);
			//replace = _header_tab.find("Host-uncut-comme-les-casquettes");
			//replace->second = get_request("Host:").substr(0, found);
			_header_tab["Host:"] =  get_request("Host:").substr(found + 1, get_request("Host:").size() - found);
			//replace = _header_tab.find("Host:");
			//replace->second = get_request("Host:").substr(found + 1, get_request("Host:").size() - found);
		}
		if ((get_request("Content-Length:").compare("") != 0 && get_request("Content-Length:").find_first_not_of("0123456789") != std::string::npos)
		|| (_buffer.rfind("Content-Length\r\n") != std::string::npos))
		{
			//std::cout << "ERROR = BAD CONTENT LENGTH" << std::endl;
			_header_tab["Status"] = "400";
			//replace = _header_tab.find("Status");
			//replace->second = "400";
		}
		else if (check_precondition() == -1)
		{
			//std::cout << "ERROR = Precondition Failed" << std::endl;
			_header_tab["Status"] = "412";
			//replace = _header_tab.find("Status");
			//replace->second = "412";
		}		
		return (1);
	}
	std::cout << "CRASSSHHHH" << std::endl;
	return (KEEP);
}
