/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/10 14:23:54by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TreatRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

TreatRequest::TreatRequest( void )
{
}

TreatRequest::TreatRequest( std::vector<Server> const & conf,
	Engine const & eng )
{
	int	comp;

	this->_eng = &eng;
	for (size_t i = 0 ; i < conf.size() ; ++i)
	{
		std::stringstream(conf[i].getListen()) >> comp;
		if (comp == this->_eng->GetAccessPort())
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

void	TreatRequest::cpyInfo( std::string const & extension,
	std::string const & path )
{
	this->_extension = extension;
	// TODO CPY LAST MODIFIED
}

void	TreatRequest::readStaticFile( std::string const & path, std::ifstream & ifs )
{
	std::string	line;

	while (std::getline(ifs, line))
		this->_file += line + "\n";
	ifs.close();
}

void	TreatRequest::readDynamicFile( std::string const & path, std::string const & pathCgi,
	Parse_request const & req )
{
	Cgi	obj_cgi(path, pathCgi, req, *this->_eng);

	obj_cgi.exec_cgi(obj_cgi.create_argv(path),
		obj_cgi.convert_env(obj_cgi.getEnv()), req);
	this->_file = obj_cgi.getSend_content();
	this->_type_cgi = obj_cgi.getType_Cgi();
	//std::cout << "this->_file\t=\t" << this->_file << std::endl;
	//dov le ashkénaze
}

bool	TreatRequest::permForOpen( std::string const & path ) const
{
	struct stat sbuf;

	int ret_stat = stat(path.c_str(), &sbuf);

	if (ret_stat == -1)
		return (false);
	std::cout << "path in permforopen = " << path << std::endl;

	if (sbuf.st_mode & S_IRUSR)
	{
		std::cout << "readable !!!" << std::endl;
		return (true);
	}
	else
		return (false);
	return (true);
	// true = perm ok / false = no perm(403)
}

bool	TreatRequest::exist_file( std::string const & path) const
{
	// true = exist  / false = no exist

	if (access(path.c_str(), F_OK) != -1) // fichier
	{
		std::cout << "Fichier existe !" << std::endl;
		return (true);
	}
	std::cout << "Fichier existe pas!" << std::endl;
	return (false);
}

bool	TreatRequest::exist_dir( std::string const & root, const Parse_request & req) const
{
	// true = exist  / false = no exist
	struct stat sbuf_dir;

	int ret_stat_dir = stat(root.c_str(), &sbuf_dir);

	if (ret_stat_dir == -1)
		return (false);
	if (S_ISDIR(sbuf_dir.st_mode)) // dossier
	{
		std::cout << "Dossier existe et perm verifie!" << std::endl;
		return (true);
	}
	std::cout << "Dossier existe pas!" << std::endl;
	return (false);
}

/* bool	TreatRequest::exist( std::string const & path, Parse_request & req) const
{
	// true = exist  / false = no exist
	struct stat sbuf_dir;

	int ret_stat_dir = stat((this->_loc->second.getRoot() + req.get_request("Path")).c_str(), &sbuf_dir);

	if (ret_stat_dir == -1)
		return (false);
	if (access(path.c_str(), F_OK) != -1) // fichier
	{
		std::cout << "Fichier existe !" << std::endl;
		return (true);
	}
	else if (S_ISDIR(sbuf_dir.st_mode)) // dossier
	{
		std::cout << "Dossier existe et perm verifie!" << std::endl;
		return (true);
	}
	std::cout << "Fichier existe pas!" << std::endl;
	return (false);
} */

bool	TreatRequest::openAndRead( std::string const & path,
	Parse_request & req )
{
	std::ifstream ifs;
	std::map<std::string, std::string>::const_iterator	it, end;
	std::string	extension;

	if (!exist_file(path))// &&
		//!exist_dir((this->_loc->second.getRoot() + req.get_request("Path")), req))
		return (false);
	if (!permForOpen(path))
	{
		req.setStatus("403");
		this->error_page(req);
		return (true);
	}

	ifs.open(path.c_str(), std::ifstream::in);
	if (!(ifs.is_open()))
		return (false);


	extension =	&path[path.rfind('/')];
	extension = &path[path.rfind('.')];
	this->_cgi = false;

	if (req.get_request("Status") != "400")
	{
		end = it = this->_loc->second.getCgi().end();
		for (it = this->_loc->second.getCgi().begin() ; it != end ; ++it)
		{
			if (extension == it->first)
			{
				this->_cgi = true;
				break;
			}
		}
	}

	if (this->_cgi)
		this->readDynamicFile(path, it->second, req);
	else
		this->readStaticFile(path, ifs);
	this->cpyInfo(extension, path);
	return (true);
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
	if (locName[i] || (i > 0 && path[i] && path[i - 1] != '/' && path[i] != '/'))
		return (0);

	return (ret);
}

std::map<std::string, Server>::const_iterator	TreatRequest::selectLocation(
	std::string const &	path,
	std::map<std::string, Server> const & allLoc )
{
	std::map<std::string, Server>::const_iterator	it, end, ret;
	size_t	similarity, most;

	most = 0;
	end = allLoc.end();
	for (it = allLoc.begin() ; it != end ; ++it)
	{
		similarity = this->similarity_point(it->first, path);
		if (similarity > most)
		{
			most = similarity;
			ret = it;
		}
	}

	return (ret);
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

bool	TreatRequest::search_index( Parse_request & req,
	std::string const & path )
{
	std::set<std::string>::const_iterator	it, end;
	std::string tmp, file;

	//std::cout << "INDEX SEARCH" << std::endl << std::endl;
	end = this->_loc->second.getIndex().end();
	for (it = this->_loc->second.getIndex().begin() ; it != end ; ++it)
	{
		tmp = path + *it;
		if (this->openAndRead(tmp, req))
			return (true);
	}
	return (false);
}

void	TreatRequest::generateAutoIndex( Parse_request & req,
	std::string const & path )
{
	std::cout << this->_loc->second.getAutoindex() << std::endl;

	if (this->is_dir(path))
	{
		std::cout << "FILE EXIST" << std::endl;
		if (!this->_loc->second.getAutoindex())
		{
			req.setStatus("403");
			this->error_page(req);
		}
		else
		{
			Autoindex	page(path.c_str(), req.get_request("Path"));

			this->_file = page.getPage();
			this->_extension = ".html";
		}
	}
	else
	{
		req.setStatus("404");
		this->error_page(req);
	}
}

void	TreatRequest::error_page( Parse_request & req )
{
	std::map<std::string, Server>::const_iterator	locErr;
	std::map<unsigned int, std::string>				allError;
	std::string			codeStr, path;
	std::stringstream	conv;
	unsigned int		code;
	bool				find_custom;

	codeStr = req.get_request("Status");
	allError = this->_loc->second.getError();

	conv << codeStr;
	conv >> code;

	find_custom = false;

	if (allError[code] != "")
	{
		locErr = this->selectLocation(allError[code], this->_conf[this->_i_conf].getLocation());
		path = locErr->second.getRoot() + locErr->first;
		if (this->openAndRead(path, req))
			find_custom = true;
	}
	if (!find_custom)
	{
		path = DEFAULT_ROOT_ERROR + codeStr + ".html";
		this->openAndRead(path, req);
	}
}

void	TreatRequest::exec_root( Parse_request & req )
{
	std::string	path = this->_loc->second.getRoot() + req.get_request("Path");

	if (path[path.length() - 1] == '/')
	{
		if (!this->search_index(req, path))
			this->generateAutoIndex(req, path);
	}
	else
	{
		if (!this->openAndRead(path, req))
		{
			req.setStatus("404");
			this->error_page(req);
		}
	}
}

void	TreatRequest::exec( Parse_request & req )
{
	std::ifstream ifs;

	ifs.open(this->_loc->second.getRoot());
	if (!(ifs.is_open()))
	{
		std::cout << "TODO ALIAS METHOD" << std::endl;
		//this->is_alias();
	}
	else
	{

		std::cout << "ROOT METHOD" << std::endl;
		ifs.close();
		exec_root(req);
	}
}

void	TreatRequest::permMethod( Parse_request & req )
{
	std::string	cmd;
	std::set<std::string>::const_iterator	it;

	cmd = req.get_request("Method");
	it = this->_loc->second.getMethods().find(cmd);
	//std::cout << "CRASH" << std::endl;
	//std::cout << "*it\t=\t" << *it << std::endl;
	if (it == this->_loc->second.getMethods().end())
	{
		std::cout << "NO METHOD" << std::endl;
		req.setStatus("405");
		this->error_page(req);
	}
	else
		this->exec(req);
}

std::string	TreatRequest::treat( Parse_request & req )
{
	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	//printMap(pol, "Tableau de merde");

	if (req.get_request("Status") == "400")
	{
		std::string path_bad_req = DEFAULT_ROOT_ERROR "400.html";
		this->openAndRead(path_bad_req, req);
	}
	else
	{
		this->_i_conf = this->selectConf(req);
		std::cout << "i_conf\t=\t" << this->_i_conf << std::endl;
		this->_loc = this->selectLocation(req.get_request("Path"), this->_conf[this->_i_conf].getLocation());
		std::cout << "location\t=\t" << _loc->first << std::endl
			<< _loc->second << std::endl;

		if (req.get_request("Status") != "200")
			this->error_page(req);
		else
			this->permMethod(req);
	}

	Response	rep(req, *this);
	//std::cout << "rep.getHeader()\t=\t" << rep.getHeader() << std::endl;
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

std::string const &	TreatRequest::getType_Cgi( void ) const
{
	return (this->_type_cgi);
}

bool	const &	TreatRequest::getIs_Cgi( void ) const
{
	return (this->_cgi);
}

/* ************************************************************************** */
