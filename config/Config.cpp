/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:45 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/07 16:42:17 by tsannie          ###   ########.fr       */
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
	/*size_t i;
	for (i = 0 ; isspace(fileStr[i]) ; ++i) {}
	if (fileStr[i] != '{')
		throw std::invalid_argument("[Error] invalid number of arguments in \"server\".");*/

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

void	Config::split_server( std::string & fileStr )
{
	size_t						pos = 0, end_pos = 0;

	while ((pos = fileStr.find("server")) != std::string::npos)
	{
		end_pos = fileStr.find("};");

		if (end_pos == std::string::npos)
			throw std::invalid_argument("[Error] a block server "
				"must be closed with '};'.");
		else if (end_pos < pos)
			throw std::invalid_argument("[Error] logic problem on the "
				"brackets '{}'.");

		std::string		to_push(fileStr.begin() + pos + 6, fileStr.begin() + end_pos);
		this->serv.push_back(Server(to_push));
		fileStr.erase(fileStr.begin() + pos, fileStr.begin() + end_pos + 2);
	}

	for (size_t i = 0 ; fileStr[i] ; ++i)
	{
		if (!isspace(fileStr[i]))
		{
			std::string thr("[Error] invalid characters have been found '");
			for (; !isspace(fileStr[i]) && fileStr[i]; ++i)
				thr += fileStr[i];
			thr += "'.";
			throw std::invalid_argument(thr);
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
