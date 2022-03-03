/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:09 by tsannie           #+#    #+#             */
/*   Updated: 2022/03/04 00:51:34 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	this->initLocation();
}

Server::Server( std::vector< std::vector<std::string> > const & src )
{
	this->initLocation();
	this->parsingAll(src);
}

Server::Server( std::string const & src )
{
	this->initServ();
	std::vector< std::vector<std::string> >	toParce;
	std::string	strParce(src.begin() + 1, src.end() - 1);

	toParce = sortInVec(strParce);
	this->parsingAll(toParce);

	this->fillLocation();
}

Server::Server( Server const & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	if ( this != &rhs )
	{
		this->_name        = rhs.getName();
		this->_index       = rhs.getIndex();
		this->_methods     = rhs.getMethods();
		this->_listen      = rhs.getListen();
		this->_root        = rhs.getRoot();
		this->_autoindex   = rhs.getAutoindex();
		this->_maxbody     = rhs.getMaxbody();
		this->_error       = rhs.getError();
		this->_cgi         = rhs.getCgi();
		this->_location    = rhs.getLocation();

		this->_alreadySetIndex     = rhs._alreadySetIndex;
		this->_alreadySetMethods   = rhs._alreadySetMethods;
		this->_alreadySetListen    = rhs._alreadySetListen;
		this->_alreadySetRoot      = rhs._alreadySetRoot;
		this->_alreadySetAutoindex = rhs._alreadySetAutoindex;
		this->_alreadySetMaxbody   = rhs._alreadySetMaxbody;
		this->_alreadySetError     = rhs._alreadySetError;
		this->_alreadySetCgi       = rhs._alreadySetCgi;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	printType(i.getName(), o, "name");
	printType(i.getIndex(), o, "index");
	printType(i.getMethods(), o, "methods");
	printType(i.getListen(), o, "listen");
	printType(i.getRoot(), o, "root");
	o << BYELLOW "autoindex : " BBLUE
		<< ((i.getAutoindex() ? "on" : "off")) << END << std::endl;
	o << BYELLOW "max_body  : " BBLUE
		<< i.getMaxbody() << END << std::endl;
	printType(i.getError(), o, "error");
	printType(i.getCgi(), o, "cgi");
	printType(i.getLocation(), o, "location");
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Server::fillLocation( void )
{
	std::map<std::string, Server>::iterator	it, end;

	end = this->_location.end();
	if (this->_location.find("/") == end)
		this->_location["/"];

	for (it = this->_location.begin() ; it != end ; ++it)
	{
		if (!it->second._alreadySetIndex)
			it->second._index = this->_index;
		if (!it->second._alreadySetMethods)
			it->second._methods = this->_methods;
		if (!it->second._alreadySetRoot)
			it->second._root = this->_root;
		if (!it->second._alreadySetAutoindex)
			it->second._autoindex = this->_autoindex;
		if (!it->second._alreadySetMaxbody)
			it->second._maxbody = this->_maxbody;
		if (!it->second._alreadySetError)
			it->second._error = this->_error;
		if (!it->second._alreadySetCgi)
			it->second._cgi = this->_cgi;
	}
}

void	Server::initLocation( void )
{
	this->_autoindex = AUTOINDEX_DEFAULT;
	this->_maxbody   = MAXBODY_DEFAULT;

	this->_alreadySetIndex     = false;
	this->_alreadySetMethods   = false;
	this->_alreadySetListen    = false;
	this->_alreadySetRoot      = false;
	this->_alreadySetAutoindex = false;
	this->_alreadySetMaxbody   = false;
	this->_alreadySetError     = false;
	this->_alreadySetCgi       = false;
}

void	Server::initServ( void )
{
	this->initLocation();

	this->_listen = std::string(LISTEN_DEFAULT);
	this->_index.insert(std::string(INDEX_DEFAULT));
	this->_root = ROOT_DEFAULT;
}

typedef void ( Server::*allFunction )( std::vector<std::string> const & );

void	Server::parsingAll(
	std::vector< std::vector<std::string> > const & src )
{
	std::string	nameAllowed[] = {"server_name", "index", "accepted_methods",
		"listen", "root", "autoindex", "client_max_body_size", "error_page",
		"cgi", "location"};
	allFunction	setFunct[] = {&Server::setName, &Server::setIndex,
		&Server::setMethods, &Server::setListen, &Server::setRoot,
		&Server::setAutoindex, &Server::setMaxbody, &Server::setError,
		&Server::setCgi, &Server::setLocation};
	std::vector< std::vector<std::string> >::const_iterator	it, end;
	bool		found;
	size_t		len, i;


	len = sizeof(nameAllowed) / sizeof(std::string);
	end = src.end();
	for (it = src.begin() ; it != end ; ++it)
	{
		found = false;
		for (i = 0 ; i < len && !found ; ++i)
		{
			if (*((*it).begin()) == nameAllowed[i])
			{
				checkNotValidDirective(*it);
				(this->*setFunct[i])(*it);
				found = true;
			}
		}
		if (!found)
		{
			std::string thr;
			thr = "[Error] unknown directive \'" + *((*it).begin()) + "\'.";
			throw std::invalid_argument(thr);
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::set<std::string> const &	Server::getName( void ) const
{
	return (this->_name);
}

std::set<std::string> const &	Server::getIndex( void ) const
{
	return (this->_index);
}

std::set<std::string> const &	Server::getMethods( void ) const
{
	return (this->_methods);
}

std::string const &				Server::getListen( void ) const
{
	return (this->_listen);
}

std::string const &				Server::getRoot( void ) const
{
	return (this->_root);
}

bool const &					Server::getAutoindex( void ) const
{
	return (this->_autoindex);
}

size_t const &					Server::getMaxbody( void ) const
{
	return (this->_maxbody);
}

std::map<unsigned int, std::string> const &	Server::getError( void ) const
{
	return (this->_error);
}

std::map<std::string, std::string> const &	Server::getCgi( void ) const
{
	return (this->_cgi);
}

std::map<std::string, Server> const &		Server::getLocation( void ) const
{
	return (this->_location);
}


void	Server::setName( std::vector<std::string> const & src )
{
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
		this->_name.insert(*it);
}

void	Server::setIndex( std::vector<std::string> const & src )
{
	std::vector<std::string>::const_iterator	it, end;

	if (!this->_alreadySetIndex)
		this->_index.clear();
	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
		this->_index.insert(*it);
	this->_alreadySetIndex = true;
}

void	Server::setMethods( std::vector<std::string> const & src )
{
	checkRedefinition(this->_alreadySetMethods, src[0]);
	std::string	methodsAllowed[] = {"GET", "POST", "DELETE"};
	std::vector<std::string>::const_iterator	it, end;
	size_t	len, i;

	len = sizeof(methodsAllowed) / sizeof(std::string);
	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
	{
		for (i = 0 ; i < len && *it != methodsAllowed[i] ; ++i) {}
		if (i == len)
			throw std::invalid_argument("[Error] invalid arguments "
				"in \'accepted_methods\'.");
		this->_methods.insert(*it);
	}
	this->_alreadySetMethods = true;
}

void	Server::setListen( std::vector<std::string> const & src )
{
	checkRedefinition(this->_alreadySetListen, src[0]);
	checkNbArg(src.size(), 2, src[0]);

	std::string	list(src[1]);

	for (size_t i = 0 ; list[i] ; ++i)
	{
		if (!std::isdigit(list[i]))
			throw std::invalid_argument("[Error] invalid listen.");
	}

	this->_alreadySetListen = true;
	this->_listen = *(++(src.begin()));
}

void	Server::setRoot( std::vector<std::string> const & src )
{
	checkRedefinition(this->_alreadySetRoot, src[0]);
	checkNbArg(src.size(), 2, src[0]);
	if (src.size() != 2)
		throw std::invalid_argument("[Error] invalid number of "
			"arguments in \'root\'.");
	this->_alreadySetRoot = true;
	this->_root = *(++(src.begin()));
}

void	Server::setAutoindex( std::vector<std::string> const & src )
{
	checkRedefinition(this->_alreadySetAutoindex, src[0]);
	checkNbArg(src.size(), 2, src[0]);
	this->_alreadySetAutoindex = true;
	if (*(++(src.begin())) != "on" && *(++(src.begin())) != "off")
		throw std::invalid_argument("[Error] invalid arguments "
			"in \'autoindex\'.");
	this->_autoindex = (*(++(src.begin())) == "on" ? true : false);
}

void	Server::setMaxbody( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetMaxbody, src[0]);
	checkNbArg(src.size(), 2, src[0]);

	this->_maxbody = stost_size(0, MAX_MAXBODY, src[1], src[0]);

	this->_alreadySetMaxbody = true;
}

void	Server::setError( std::vector<std::string> const & src )
{
	checkNbArgMin(src.size(), 3, src[0]);
	std::pair<std::map<unsigned int, std::string>::iterator, bool>	ret;
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it + 1 != end ; ++it)
	{
		ret = this->_error.insert(std::make_pair( stost_size(300, 599, *it,
			src[0]), *(end - 1) ));
		if (ret.second == false)
		{
			std::string thr("[Error] page for error code \'");
			thr += *it + "\' is already defined in \'" + src[0] + "\'.";
			throw std::invalid_argument(thr);
		}
	}
	this->_alreadySetError = true;
}

void	Server::setCgi( std::vector<std::string> const & src )
{
	checkNbArg(src.size(), 3, src[0]);
	std::pair<std::map<std::string, std::string>::iterator, bool>	ret;
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it + 1 != end ; ++it)
	{
		ret = this->_cgi.insert(std::make_pair(src[1], src[2]));
		if (ret.second == false)
		{
			std::string thr("[Error] page for error code \'");
			thr += *it + "\' is already defined in \'" + src[0] + "\'.";
			throw std::invalid_argument(thr);
		}
	}
	this->_alreadySetCgi = true;
}

void	Server::setLocation( std::vector<std::string> const & src )
{
	checkNbArg(src.size(), 3, src[0]);
	std::pair<std::map<std::string, Server>::iterator, bool>	ret;
	std::vector< std::vector<std::string> >::const_iterator	it, end;

	std::string	strParce(src[2].begin() + 1, src[2].end() - 1);
	std::vector< std::vector<std::string> >		newConstruct;
	std::string	nameForbidden[] = {"server_name", "listen", "location"};
	size_t		len, i;

	newConstruct = sortInVec(strParce);
	len = sizeof(nameForbidden) / sizeof(std::string);
	end = newConstruct.end();
	for (it = newConstruct.begin() ; it != end ; ++it)
	{
		for (i = 0 ; i < len ; ++i)
		{
			if (*((*it).begin()) == nameForbidden[i])
			{
				std::string thr("[Error] \'");
				thr += nameForbidden[i]
					+ "\' directive cannot be defined in location.";
				throw std::invalid_argument(thr);
			}
		}
	}

	ret = this->_location.insert(std::make_pair(src[1], Server(newConstruct)));
	if (ret.second == false)
	{
		std::string thr("[Error] location \'");
		thr += src[1] + "\' is duplicate.";
		throw std::invalid_argument(thr);
	}

}


/* ************************************************************************** */
