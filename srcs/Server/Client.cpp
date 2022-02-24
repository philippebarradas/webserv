
#include "Client.hpp"
#include "../Cgi/Cgi.hpp"
#include "../Parse_request/parse_request.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


Client::Client(epoll_event & ev)
{
	_parse_head = new Parse_request();
	_recv_len = 0;
	_request_header_size = 0;
	_header_parsed = false;
	_is_sendable = false;
	_fill_request = "";
	_events = ev;
	_fd = _events.data.fd;

	std::map<std::string, std::string>::const_iterator it, end;
	//std::map<std::string, std::string> map = _parse_head->getBigMegaSuperTab();

	/* end = _parse_head->getBigMegaSuperTab().end();
	for ( it = _parse_head->getBigMegaSuperTab().begin() ; it != end ; ++it)
	{
		//if (it->second.size() != 0)
		std::cout << YELLOW << "[" << it->first << "] = [" << it->second << "]" << END << std::endl;
	} */
}

Client::Client(Client const & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
	delete _parse_head;
	std::cout << GREEN << "----------------- End of Client -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client&				Client::operator=( Client const & rhs )
{
	if (this != &rhs)
	{
		this->_parse_head = new Parse_request();
		this->_recv_len = rhs._recv_len;
		this->_is_sendable = rhs._is_sendable;
		this->_header_parsed = rhs._header_parsed;
		this->_fill_request = rhs._fill_request;
		this->_request_header_size = rhs._request_header_size;
		this->_events = rhs._events;
		*this->_parse_head = *rhs._parse_head;
		this->_fd = rhs._fd;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Client::setRecv_len(const int & recv_len)
{
	this->_recv_len += recv_len;
}

void	Client::setRequest_header_size(const size_t & header_size)
{
	this->_request_header_size = header_size;
}

void	Client::setFill_request(const char & fill_request)
{
	this->_fill_request += fill_request;
}

void	Client::setHeader_parsed(const bool & header_parsed)
{
	this->_header_parsed = header_parsed;
}

void	Client::setIs_sendable(bool const & is_sendable)
{
	this->_is_sendable = is_sendable;
}

void		Client::setFd(int const & fd)
{
	this->_fd = fd;
}

Parse_request & Client::getParse_head()
{
	return (*this->_parse_head);
}

epoll_event & Client::getEvents()
{
	return (this->_events);
}

int	& Client::getRecv_len()
{
	return (this->_recv_len);
}

size_t & Client::getRequest_header_size()
{
	return (this->_request_header_size);
}

std::string & Client::getFill_request()
{
	return (this->_fill_request);
}

bool	& Client::getHeader_parsed()
{
	return (this->_header_parsed);
}

bool	& Client::getIs_sendable()
{
	return (this->_is_sendable);
}

int		& Client::getFd()
{
	return (this->_fd);
}
