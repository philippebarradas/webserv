/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:09 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/09 19:11:05 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

bool	Server::_alreadySetMethods   = false;
bool	Server::_alreadySetListen    = false;
bool	Server::_alreadySetRoot      = false;
bool	Server::_alreadySetAutoindex = false;
bool	Server::_alreadySetMaxbody   = false;

Server::Server()
{
}

Server::Server( std::string const & src )
{
	size_t	i;
	std::set< std::vector<std::string> >	toParce;
	std::string	strParce;

	for (i = 0 ; isspace(src[i]) && src[i] ; ++i) {}
	if (src[i] != '{')
	{
		std::string thr("[Error] invalid characters after server bloc '");
		for (; !isspace(src[i]) && src[i] != '{' && src[i]; ++i)
			thr += src[i];
		thr += "'.";
		throw std::invalid_argument(thr);
	}
	strParce = std::string(&src[i + 1]);
	toParce = sortInVec(strParce);
	this->parsingAll(toParce);

	std::cout << std::endl << *this << std::endl;
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
		this->_server_name = rhs.getName();
		this->_index       = rhs.getIndex();
		this->_listen      = rhs.getListen();
		this->_root        = rhs.getRoot();
		this->_autoindex   = rhs.getAutoindex();
		this->_maxbody     = rhs.getMaxbody();
		this->_error       = rhs.getError();
	}
	return *this;
}

template <typename T>
void	printContainers(T const & ctn, std::ostream & o)
{
	typename	T::const_iterator	it, end;

	o << "[ ";
	end = ctn.end();
	for (it = ctn.begin() ; it != end ; ++it)
		o << "\'" << *it << "\' ";
	o << "]" << std::endl;
}

template <typename T1, typename T2>
void	printContainers(std::map<T1, T2> const & map, std::ostream & o)
{
	typename	std::map<T1, T2>::const_iterator	it, end;

	o << "[ ";
	end = map.end();
	for (it = map.begin() ; it != end ; ++it)
		o << "{\'" << it->first << "\' => \'" << it->second << "\'} ";
	o << "]" << std::endl;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << "name      : ";
	printContainers(i.getName(), o);
	o << "index     : ";
	printContainers(i.getIndex(), o);
	o << "methods   : ";
	printContainers(i.getMethods(), o);
	o << "listen    : "
	<< i.getListen() << std::endl;
	o << "root      : "
	<< i.getRoot() << std::endl;
	o << "autoindex : "
	<< ((i.getAutoindex() ? "on" : "off")) << std::endl;
	o << "max_body  : "
	<< i.getMaxbody() << std::endl;
	o << "error     : ";
	printContainers(i.getError(), o);
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

typedef void ( Server::*allFunction )( std::vector<std::string> const & );

void	Server::parsingAll( std::set< std::vector<std::string> > const & src )
{
	std::string	nameAllowed[] = {"server_name", "index", "accepted_methods",
		"listen", "root", "autoindex", "client_max_body_size", "error_page"};
	allFunction	setFunct[] = {&Server::setName, &Server::setIndex,
		&Server::setMethods, &Server::setListen, &Server::setRoot,
		&Server::setAutoindex, &Server::setMaxbody, &Server::setError};
	std::set< std::vector<std::string> >::const_iterator	it, end;
	bool		found;
	size_t		len, i;

	len = sizeof(nameAllowed) / sizeof(std::string);
	end = src.end();
	for (it = src.begin() ; it != end ; ++it)
	{
		found = false;
		for (i = 0 ; i < len && !found ; i++)
		{
			if (*((*it).begin()) == nameAllowed[i])
			{
				(this->*setFunct[i])(*it);
				found = true;
			}
		}
		/*if (!found) // if vec.size < 2 // brackets
			std::cout << *((*it).begin()) << " is not found." << std::endl;
		else
			std::cout << *((*it).begin()) << " is found." << std::endl;*/
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::set<std::string>	Server::getName() const
{
	return (this->_server_name);
}

std::set<std::string>	Server::getIndex() const
{
	return (this->_index);
}

std::set<std::string>	Server::getMethods() const
{
	return (this->_methods);
}

std::string				Server::getListen() const
{
	return (this->_listen);
}

std::string				Server::getRoot() const
{
	return (this->_root);
}

bool					Server::getAutoindex() const
{
	return (this->_autoindex);
}

unsigned int			Server::getMaxbody() const
{
	return (this->_maxbody);
}

std::map<unsigned int, std::string>	Server::getError() const
{
	return (this->_error);
}

void	Server::setName( std::vector<std::string> const & src )
{
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
		this->_server_name.insert(*it);
}

void	Server::setIndex( std::vector<std::string> const & src )
{
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
		this->_index.insert(*it);
}

void	Server::setMethods( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetMethods, src[0]);
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
	_alreadySetMethods = true;
}

void	Server::setListen( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetListen, src[0]);
	checkNbArg(src.size(), 2, src[0]);
	// autoriser les char ?? le test listen e; prouve que oui ??
	_alreadySetListen = true;
	this->_listen = *(++(src.begin()));
}

void	Server::setRoot( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetRoot, src[0]);
	checkNbArg(src.size(), 2, src[0]);
	if (src.size() != 2)
		throw std::invalid_argument("[Error] invalid number of "
			"arguments in \'root\'.");
	_alreadySetRoot = true;
	this->_root = *(++(src.begin()));
}

void	Server::setAutoindex( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetAutoindex, src[0]);
	checkNbArg(src.size(), 2, src[0]);
	_alreadySetAutoindex = true;
	this->_autoindex = (*(++(src.begin())) == "on" ? true : false);
}

void	Server::setMaxbody( std::vector<std::string> const & src )
{
	checkRedefinition(_alreadySetMaxbody, src[0]);
	checkNbArg(src.size(), 2, src[0]);

	this->_maxbody = stoui_size(0, 100, src[1], src[0]);

	_alreadySetMaxbody = true;
}

void	Server::setError( std::vector<std::string> const & src )
{
	std::pair<std::map<unsigned int, std::string>::iterator, bool>	ret;
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it + 1 != end ; ++it)
	{
		ret = this->_error.insert(std::make_pair( stoui_size(300, 599, *it,
			src[0]), *(end - 1) ));
		if (ret.second == false)
		{
			std::string thr("[Error] page for error code \'");
			thr += *it + "\' is already defined in \'" + src[0] + "\'.";
			throw std::invalid_argument(thr);
		}
	}
}

/* ************************************************************************** */
