
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
	_events = ev;
	fd = _events.data.fd;

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
	//std::cout << GREEN << "----------------- End of Client -----------------" << END << std::endl << std::endl;
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
		fd = rhs.fd;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client & i )
{
	std::cout << "ope" << std::endl;
	o << RED << i.fill_request << std::endl;
	o << "fd =" << i.fd << std::endl;
	o << i.getEvents().data.fd << std::endl;
	std::cout << "after" << std::endl;
	o << i.getEvents().events << std::endl << END;
	return o;
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
	return (this->_events);
}
