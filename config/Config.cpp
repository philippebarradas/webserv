/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:45 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/06 20:42:25 by tsannie          ###   ########.fr       */
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
	std::ifstream ifs;

	ifs.open (file_name, std::ifstream::in);

	std::string	line, file;

	while (std::getline(ifs, line)) {
		file += line;
		file += "\n";		// to remove
	}

	split_server(file);

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

std::vector<std::string>	Config::split_server( std::string const & fileStr ) const
{
	std::vector<std::string>	ret;
	size_t						pos = 0;

	while ((pos = fileStr.find("server")))
	{
		std::cout << "start = " << pos << std::endl;
		return ret;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
