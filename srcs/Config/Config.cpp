/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:45 by tsannie           #+#    #+#             */
/*   Updated: 2022/03/01 15:26:43 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config()
{

}

Config::Config( const char *file_name )
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
	this->parsingAll(toParce);

	ifs.close();
}

Config::Config( const Config & src )
{
	*this = src;
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
	if ( this != &rhs )
	{
		this->_config = rhs.getConfig();
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Config const & i )
{
	size_t e, y;
	for (e = 0 ; e < i.getConfig().size() ; ++e)
	{
		o << BGREEN "Config server #" << (e + 1)
			<< ":\n" END << std::endl;
		o << i.getConfig()[e] << std::endl;
		for (y = 0 ; y < 50 ; ++y)
			o << '-';
		o << std::endl;
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Config::parsingAll( std::vector< std::vector<std::string> > const & src )
{
	std::vector< std::vector<std::string> >::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() ; it != end ; ++it)
	{
		if (*((*it).begin()) == "server")
		{
			checkNotValidDirective(*it);
			this->_config.push_back(Server(*((*it).begin() + 1)));
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

std::vector<Server> const &	Config::getConfig() const
{
	return (this->_config);
}

/* ************************************************************************** */
