/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/25 16:27:35 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TreatRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

TreatRequest::TreatRequest( void )
{
}

TreatRequest::TreatRequest( std::vector<Server> const & conf, int const & access_port )
{
	int	comp;

	for (size_t i = 0 ; i < conf.size() ; ++i)
	{
		std::stringstream(conf[i].getListen()) >> comp;
		if (comp == access_port)
			this->_conf.push_back(conf[i]);
	}
}

TreatRequest::TreatRequest( TreatRequest const & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

TreatRequest::~TreatRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

TreatRequest &				TreatRequest::operator=( TreatRequest const & rhs )
{
	if ( this != &rhs )
	{
		this->_conf = rhs._conf;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

// DISPLAY (TO DELETE)
template <typename T>
void printMap(T & map, std::string const & name)
{
	typename	T::iterator	it;
	typename	T::iterator	end;

	std::cout << "----------------" << std::endl;
	std::cout << name << " contains:" << std::endl;

	end = map.end();
	for (it = map.begin() ; it != end ; it++)
		std::cout << it->first << " => " << it->second << std::endl;
	std::cout << "size = " << map.size() << std::endl;
	std::cout << "----------------\n" << std::endl;
}

std::string	TreatRequest::openAndRead( std::string const & path ) const
{
	std::ifstream ifs;
	std::string	line, file;

    ifs.open(path.c_str(), std::ifstream::in);
	if (!(ifs.is_open()))
		throw std::runtime_error("Error 404");
	while (std::getline(ifs, line))
		file += line + "\n";
	ifs.close();

	return (file);
}

std::string	TreatRequest::printError( Parse_header const & req,
	size_t const & i_conf ) const
{
	std::string	path_default_error, file;


	path_default_error = "srcs/Config/default/html_page/error_page/"
		+ req.get_request("status") + ".html";
	file = openAndRead(path_default_error);

	Response	rep(req, file, std::string(".html"));
	return (rep.getHeader());
}

size_t		TreatRequest::selectConf( Parse_header const & req ) const
{
	size_t i;
	std::set<std::string>::const_iterator	it, end;

	for (i = 0 ; i < this->_conf.size() ; ++i)
	{
		end = this->_conf[i].getName().end();
		for (it = this->_conf[i].getName().begin() ; it != end ; ++it)
		{
			if (req.get_request("Host") == *it)
				return (i);
		}
	}
	return (0);
}

std::string	TreatRequest::exec( Parse_header const & req )
{
	size_t	i_conf;

	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	printMap(pol, "fddf");

	i_conf = this->selectConf(req);

	if (req.get_request("status") != "200")
		return (printError( req ));
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
