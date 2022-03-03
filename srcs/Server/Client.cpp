
#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(int & fd)
{
	_parse_head.push_back(Parse_request());
	_recv_len = 0;
	_request_header_size = 0;
	_header_parsed = false;
	_header_readed = false;
	_is_sendable = false;
	_fill_request = "";
	_fd = fd;
}

Client::Client(epoll_event & ev)
{
	_parse_head.push_back(Parse_request());
	_recv_len = 0;
	_request_header_size = 0;
	_header_parsed = false;
	_header_readed = false;
	_is_sendable = false;
	_fill_request = "";
	_events = ev;
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
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client&				Client::operator=( Client const & rhs )
{
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_recv_len = rhs._recv_len;
		this->_is_sendable = rhs._is_sendable;
		this->_header_parsed = rhs._header_parsed;
		this->_header_readed = rhs._header_readed;
		this->_fill_request = rhs._fill_request;
		this->_request_header_size = rhs._request_header_size;
		this->_events = rhs._events;
		if (rhs._parse_head.size())
			this->_parse_head = rhs._parse_head;
		else
			this->_parse_head.push_back(Parse_request());
	}
	return *this;
}

/*
** --------------------------------- SETTERS ----------------------------------
*/

void	Client::setRecv_len(size_t const & recv_len)
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

void	Client::setFill_request_body(const char *fill_request, const size_t & valread)
{
	this->_fill_request.append(fill_request, valread);
}

void	Client::setHeader_parsed(const bool & header_parsed)
{
	this->_header_parsed = header_parsed;
}

void	Client::setHeader_readed(const bool & header_readed)
{
	this->_header_readed = header_readed;
}

void	Client::setIs_sendable(bool const & is_sendable)
{
	this->_is_sendable = is_sendable;
}

void	Client::setFd(const int & fd)
{
	this->_fd = fd;
}

/*
** --------------------------------- GETTERS ----------------------------------
*/

Parse_request	& Client::getParse_head()
{
	return (this->_parse_head[0]);
}

epoll_event	& Client::getEvents()
{
	return (this->_events);
}

size_t	const & Client::getRecv_len() const
{
	return (this->_recv_len);
}

size_t	const & Client::getRequest_header_size() const
{
	return (this->_request_header_size);
}

std::string	const & Client::getFill_request() const
{
	return (this->_fill_request);
}

bool	const & Client::getHeader_parsed() const
{
	return (this->_header_parsed);
}

bool	const & Client::getHeader_readed() const
{
	return (this->_header_readed);
}

bool	const & Client::getIs_sendable() const
{
	return (this->_is_sendable);
}

int		const & Client::getFd() const
{
	return (this->_fd);
}
