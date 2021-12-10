/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:45 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/10 19:25:03 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config()
{
}

Config::Config( std::string const & file_name )
{
	std::vector< std::vector<std::string> >	toParce;
	std::ifstream ifs;
	std::string	line, file;

	ifs.open(file_name, std::ifstream::in);
	if (!ifs.is_open())
		throw std::invalid_argument("[Error] opening file");
	while (std::getline(ifs, line))
		file += line;

	toParce = sortInVec(file);
	this->split_server(toParce);

	ifs.close();
}

Config::Config( const Config & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::~Config()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Config &				Config::operator=( Config const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Config const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Config::split_server( std::vector< std::vector<std::string> > const & src )
{
	std::vector< std::vector<std::string> >::const_iterator	it, end;
	size_t		i;

	end = src.end();
	for (it = src.begin() ; it != end ; ++it)
	{
		if (*((*it).begin()) == "server")
		{
			checkNotValidDirective(*it);
			Server(*((*it).begin() + 1));
		}
		else
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


/* ************************************************************************** */
