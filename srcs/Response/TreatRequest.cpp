/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/07 16:39:41 by tsannie          ###   ########.fr       */
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
		throw std::runtime_error("404");
	while (std::getline(ifs, line))
		file += line + "\n";
	ifs.close();

	return (file);
}

std::string	TreatRequest::printError( Parse_request const & req,
	size_t const & i_conf ) const
{
	/*std::string	path_default_error, file;


	path_default_error = "srcs/Config/default/html_page/error_page/"
		+ req.get_request("status") + ".html";
	file = openAndRead(path_default_error);

	Response	rep(req, file, std::string(".html"));
	return (rep.getHeader());*/
}

size_t		TreatRequest::selectConf( Parse_request const & req ) const
{
	size_t i;
	std::set<std::string>::const_iterator	it, end;
	std::set<std::string>	name;

	for (i = 0 ; i < this->_conf.size() ; ++i)
	{
		name = this->_conf[i].getName();
		end = name.end();
		for (it = name.begin() ; it != end ; ++it)
		{
			if (req.get_request("Host:") == *it)
				return (i);
		}
	}
	return (0);
}

size_t	TreatRequest::similarity_point(std::string const & locName,
	std::string const & path) const
{
	size_t	ret, i;

	for (i = 0, ret = 0 ; locName[i] && path[i] ; ++i, ++ret)
	{
		if (locName[i] != path[i])
			return (0);
	}

	/*std::cout << "locName[i] : " << (locName[i] ? "true" : "false") << std::endl;
	std::cout << "path[i]    : " << (path[i] ? "true" : "false") << std::endl;
	std::cout << "path[i]\t=\t" << path[i] << std::endl;*/

	if (locName[i] || (i > 0 && path[i] && path[i - 1] != '/' && path[i] != '/'))
		return (0);
	/*std::cout << "locName\t=\t" << locName << std::endl;
	std::cout << "path\t=\t" << path << std::endl;
	std::cout << "ret\t=\t" << ret << std::endl;
	std::cout << "path[i]\t=\t" << path[i] << std::endl;
	std::cout << std::endl;*/

	return (ret);
}

void	TreatRequest::selectLocation(
	std::map<std::string, Server>::const_iterator & loc,
	Parse_request const & req,
	std::map<std::string, Server> const & allLoc ) const
{
	std::map<std::string, Server>::const_iterator	it, end;
	size_t	similarity, most;

	most = 0;
	end = allLoc.end();
	for (it = allLoc.begin() ; it != end ; ++it)
	{
		similarity = this->similarity_point(it->first, req.get_request("Path"));
		//std::cout << "similarity\t=\t" << similarity << std::endl;
		if (similarity > most)
		{
			most = similarity;
			loc = it;
		}
	}
}

bool	TreatRequest::is_dir( std::string const & path ) const
{
	DIR *pDir;

	pDir = opendir(path.c_str());
	if (pDir == NULL)
		return false;
	closedir(pDir);
	return true;
}

void	TreatRequest::search_index( Parse_request const & req,
	std::map<std::string, Server>::const_iterator const & loc,
	std::string const & path )
{
	std::set<std::string>::const_iterator	it, end;
	std::string tmp, file;

	end = loc->second.getIndex().end();
	for (it = loc->second.getIndex().begin() ; it != end ; ++it)
	{
		try
		{
			tmp = path + *it;
			std::cout << "tmp\t=\t" << tmp << std::endl;
			this->_file = this->openAndRead(tmp);
		}
		catch (std::runtime_error const & e)
		{
			(void)e;
		}
	}
	throw std::invalid_argument("Not found");
}

void	TreatRequest::exec_root( Parse_request const & req,
	std::map<std::string, Server>::const_iterator const & loc )
{
	std::string	path = loc->second.getRoot() + req.get_request("Path");

	std::cout << "path\t=\t" << path << std::endl;

	if (path[path.length() - 1] == '/')
	{
		try
		{
			this->search_index(req, loc, path);
		}
		catch (std::invalid_argument const & e)
		{
			std::cout << "TODO AUTOINDEX" << std::endl;
			(void)e;
		}
	}
	else
	{
		try
		{
			this->_file = this->openAndRead(path);
		}
		catch (std::runtime_error const & e)
		{
			std::cout << "TODO REDIRECT ERROR PAGE" << std::endl;
			(void)e;
		}
	}
}

void	TreatRequest::exec( Parse_request const & req,
	std::map<std::string, Server>::const_iterator const & loc )
{
	std::ifstream ifs;

	std::cout << "loc->second.getRoot()\t=\t" << loc->second.getRoot() << std::endl;

	ifs.open(loc->second.getRoot());
	if (!(ifs.is_open()))
	{
		std::cout << "ALIAS METHOD" << std::endl;
		//this->is_alias();
	}
	else
	{

		std::cout << "ROOT METHOD" << std::endl;
		ifs.close();
		std::cout << "is_dir(file)\t=\t" << is_dir(loc->second.getRoot()) << std::endl;
		exec_root(req, loc);
	}
}

std::string	TreatRequest::treat( Parse_request const & req )
{
	std::map<std::string, Server>::const_iterator	loc;
	size_t	i_conf;

	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	printMap(pol, "Tableau de merde");

	i_conf = this->selectConf(req);
	std::cout << "i_conf\t=\t" << i_conf << std::endl;
	this->selectLocation(loc, req, this->_conf[i_conf].getLocation());
	std::cout << "location\t=\t" << loc->first << std::endl;
	std::cout << "loc->second\t=\t" << loc->second << std::endl;


	if (req.get_request("Status") == "200")
	{
		this->exec(req, loc);
	}
	else
		std::cout << "TODO CAS D'ERREUR" << std::endl;

	Response	rep(req, this->_file);
	return (rep.getHeader());
	/*else (req.get_request("status") != "200")
		return (printError( req, i_conf ));*/
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
