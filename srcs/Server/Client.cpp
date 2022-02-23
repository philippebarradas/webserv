
#include "Client.hpp"
#include "../Cgi/Cgi.hpp"
#include "../Parse_request/parse_request.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


Client::Client(epoll_event & ev)
{
	_parse_head = new Parse_request();
	recv_len = 0;
	request_header_size = 0;
	is_parsed = false;
	is_sendable = false;
	fill_request = "";
	_events = &ev;

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
		this->recv_len = rhs.recv_len;
		this->is_sendable = rhs.is_sendable;
		this->is_parsed = rhs.is_parsed;
		this->fill_request = rhs.fill_request;
		this->request_header_size = rhs.request_header_size;
		this->_events = rhs._events;
		*this->_parse_head = *rhs._parse_head;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

Parse_request & Client::getParse_head()
{
	return (*this->_parse_head);
}

epoll_event & Client::getEvents()
{
	return (*this->_events);
}
