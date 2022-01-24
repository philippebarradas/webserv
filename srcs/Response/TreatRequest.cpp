/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/24 19:08:55 by tsannie          ###   ########.fr       */
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
			this->_conf = &conf[i];
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

std::string	TreatRequest::printError( Parse_header const & req )
{
	std::string	path_default_error;
	std::ifstream ifs;
	std::string	line, file;

	path_default_error = "srcs/Config/default/html_page/error_page/"
		+ req.get_request("status") + ".html";

	std::cout << "path_default_error\t=\t" << path_default_error << std::endl;

    ifs.open(path_default_error.c_str(), std::ifstream::in);
	/*if (!(ifs.is_open()))
		std::cout << "not found" << std::endl;*/
	while (std::getline(ifs, line))
		file += line + "\n";
	ifs.close();
	//std::cout << "file\t=\t" << file << std::endl;

	Response	rep(req, file, std::string(".html"));
	return (rep.getHeader());
}


std::string	TreatRequest::exec( Parse_header const & req )
{
	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	printMap(pol, "fddf");

	if (req.get_request("status") != "200")
		return (printError( req ));
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
