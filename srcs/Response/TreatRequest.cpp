/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/08 13:08:19 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TreatRequest.hpp"
#include "Cgi/Cgi.hpp"

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

void	TreatRequest::cpyInfo( std::ifstream const & ifs,
	std::string const & path )
{
	this->_extension =	&path[path.rfind('/')];
	this->_extension = &this->_extension[this->_extension.length() - 5];
}

void	TreatRequest::readStaticFile( std::string const & path, std::ifstream & ifs )
{
	std::string	line;

	while (std::getline(ifs, line))
		this->_file += line + "\n";
	ifs.close();
}

void	TreatRequest::readDynamicFile( std::string const & path, std::string const & pathCgi )
{
	Cgi	obj_cgi();

	
	std::cout << "TODO DYNAMIC FILE" << std::endl;
	std::cout << "path\t=\t" << path << std::endl;
	std::cout << "pathCgi\t=\t" << pathCgi << std::endl;
	//dov le ashkénaze
}

void	TreatRequest::openAndRead( std::string const & path )
{
	std::ifstream ifs;
	std::map<std::string, std::string>::const_iterator	it, end;
	std::string	extension;
	bool	is_dynamic;

	ifs.open(path.c_str(), std::ifstream::in);
	if (!(ifs.is_open()))
		throw std::runtime_error("404");

	extension =	&path[path.rfind('/')];
	extension = &path[path.rfind('.')];
	is_dynamic = false;
	end = it = this->_loc->second.getCgi().end();
	for (it = this->_loc->second.getCgi().begin() ; it != end ; ++it)
	{
		std::cout << "extension\t=\t" << extension << std::endl;
		std::cout << "it->first\t=\t" << it->first << std::endl;
		if (extension == it->first)
		{
			is_dynamic = true;
			break;
		}
	}

	std::cout << "is_dynamic\t=\t" << is_dynamic << std::endl;

	if (is_dynamic)
		this->readDynamicFile(path, it->second);
	else
		this->readStaticFile(path, ifs);
	//this->cpyInfo(ifs, path);
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
	Parse_request const & req,
	std::map<std::string, Server> const & allLoc )
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
			this->_loc = it;
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
	std::string const & path )
{
	std::set<std::string>::const_iterator	it, end;
	std::string tmp, file;

	std::cout << "INDEX SEARCH" << std::endl << std::endl;
	end = this->_loc->second.getIndex().end();
	for (it = this->_loc->second.getIndex().begin() ; it != end ; ++it)
	{
		try
		{
			tmp = path + *it;
			std::cout << "tmp\t=\t" << tmp << std::endl;
			this->openAndRead(tmp);
		}
		catch (std::runtime_error const & e)
		{
			(void)e;
		}
	}
	throw std::invalid_argument("Not found");
}

void	TreatRequest::exec_root( Parse_request const & req)
{
	std::string	path = this->_loc->second.getRoot() + req.get_request("Path");

	std::cout << "path\t=\t" << path << std::endl;

	if (path[path.length() - 1] == '/')
	{
		try
		{
			this->search_index(req, path);
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
			this->openAndRead(path);
		}
		catch (std::runtime_error const & e)
		{
			std::cout << "TODO REDIRECT ERROR PAGE" << std::endl;
			(void)e;
		}
	}
}

void	TreatRequest::exec( Parse_request const & req)
{
	std::ifstream ifs;

	std::cout << "loc->second.getRoot()\t=\t" << _loc->second.getRoot() << std::endl;

	ifs.open(this->_loc->second.getRoot());
	if (!(ifs.is_open()))
	{
		std::cout << "ALIAS METHOD" << std::endl;
		//this->is_alias();
	}
	else
	{

		std::cout << "ROOT METHOD" << std::endl;
		ifs.close();
		//std::cout << "is_dir(file)\t=\t" << is_dir(this->_loc->second.getRoot()) << std::endl;
		exec_root(req);
	}
}

std::string	TreatRequest::treat( Parse_request const & req )
{
	size_t	i_conf;

	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	printMap(pol, "Tableau de merde");

	i_conf = this->selectConf(req);
	std::cout << "i_conf\t=\t" << i_conf << std::endl;
	this->selectLocation(req, this->_conf[i_conf].getLocation());
	std::cout << "location\t=\t" << _loc->first << std::endl;
	std::cout << "loc->second\t=\t" << _loc->second << std::endl;


	if (req.get_request("Method") == "GET")
		this->exec(req);
	else
		std::cout << "TODO CAS D'ERREUR" << std::endl;

	Response	rep(req, *this);
	std::cout << "rep.getHeader()\t=\t" << rep.getHeader() << std::endl;
	return (rep.getHeader());
	/*else (req.get_request("status") != "200")
		return (printError( req, i_conf ));*/
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &	TreatRequest::getExtension( void ) const
{
	return (this->_extension);
}

std::string const &	TreatRequest::getFile( void ) const
{
	return (this->_file);
}

/* ************************************************************************** */
