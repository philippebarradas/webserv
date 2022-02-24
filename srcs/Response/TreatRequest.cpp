/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/24 13:33:29 by tsannie          ###   ########.fr       */
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
		this->_conf       = rhs._conf;
		this->_loc        = rhs._loc;
		this->_eng        = rhs._eng;
		this->_file       = rhs._file;
		this->_location   = rhs._location;
		this->_type_cgi   = rhs._type_cgi;
		this->_last_modif = rhs._last_modif;
		this->_extension  = rhs._extension;
		this->_cgi        = rhs._cgi;
		this->_i_conf     = rhs._i_conf;
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

// DISPLAY (TODE DELETE)
template <typename T>
void printMap(T & map, std::string const & name)
{
/* 	typename	T::iterator	it;
	typename	T::iterator	end;

	//std::cout << "----------------" << std::endl;
	//std::cout << name << " contains:" << std::endl;

	end = map.end();
	for (it = map.begin() ; it != end ; it++)
		std::cout << it->first << " => " << it->second << std::endl;
	std::cout << "size = " << map.size() << std::endl;
	std::cout << "----------------\n" << std::endl; */
}

bool	TreatRequest::check_precondition( Parse_request const & req,
	struct tm const & timefile ) const
{
	std::string	time_unmodified;
	struct tm	timereq;

	time_unmodified = req.get_request("If-Unmodified-Since:");
	if (time_unmodified == "")
		return (true);

	strptime(time_unmodified.c_str(), "%a, %d %b %Y %T %Z", &timereq);

	if (timefile.tm_year != timereq.tm_year)
		return (timefile.tm_year < timereq.tm_year);
	if (timefile.tm_yday != timereq.tm_yday)
		return (timefile.tm_yday < timefile.tm_yday);
	if (timefile.tm_hour != timefile.tm_hour)
		return (timefile.tm_hour < timefile.tm_hour);
	if (timefile.tm_min != timefile.tm_min)
		return (timefile.tm_min < timefile.tm_min);
	if (timefile.tm_sec != timefile.tm_sec)
		return (timefile.tm_sec < timefile.tm_sec);
	return (true);
}

void	TreatRequest::cpyInfo( std::string const & extension,
	std::string const & path, Parse_request & req )
{
	struct tm *	timeinfo;
	struct stat	result;
	char		time_modified_file[50];

	if (req.get_request("Status") == "200" && !this->_cgi)
	{
		stat(path.c_str(), &result);
		timeinfo = localtime(&result.st_ctim.tv_sec);
		strftime(time_modified_file, 50, "%a, %d %b %Y %T %Z", timeinfo);
		if (!check_precondition(req, *timeinfo))
		{
			this->_file.clear();
			req.setStatus("412");
			this->error_page(req);
			return ;
		}
		this->_last_modif = std::string(time_modified_file);
	}
	this->_extension = extension;
}

void	TreatRequest::readStaticFile( std::ifstream & ifs )
{
	std::string	line;

	while (std::getline(ifs, line))
		this->_file += line + "\n";
	ifs.close();
}

void	TreatRequest::readDynamicFile( std::string const & path,
	std::string const & pathCgi,
	Parse_request const & req )
{
	Cgi	obj_cgi(this->_loc->second.getRoot(), path, pathCgi, req, *this->_eng);

	obj_cgi.exec_cgi(obj_cgi.create_argv(path),
		obj_cgi.convert_env(obj_cgi.getEnv()), req);

	this->_file = obj_cgi.getSend_content();

	this->_type_cgi = obj_cgi.getType_Cgi();
}

void	TreatRequest::force_open( Parse_request const & req )
{
	std::ifstream	ifs;
	std::string		pathErr;

	pathErr = DEFAULT_ROOT_ERROR + req.get_request("Status") + ".html";
	ifs.open(pathErr, std::ifstream::in);
	this->readStaticFile(ifs);
	this->_extension = ".html";
	this->_cgi = false;
}

bool	TreatRequest::permForOpen( std::string const & path ) const
{
	struct stat sbuf;

	int ret_stat = stat(path.c_str(), &sbuf);

	if (ret_stat == -1)
		return (false);
	return (sbuf.st_mode & S_IRUSR);
}

bool	TreatRequest::exist_file( std::string const & path) const
{
	struct stat path_stat;

	if (stat(path.c_str(), &path_stat) == -1)
		return (false);
	return (S_ISREG(path_stat.st_mode));
}

bool	TreatRequest::exist_dir( std::string const & root) const
{
	struct stat	sbuf_dir;

	if (stat(root.c_str(), &sbuf_dir) == -1)
		return (false);
	return (S_ISDIR(sbuf_dir.st_mode));
}

bool	TreatRequest::exist( std::string const & root) const
{
	if (this->exist_dir(root) || this->exist_file(root))
		return (true);
	return (false);
}

bool	TreatRequest::check502( std::string const & pathCgi,
	Parse_request & req )
{
	std::string	pathcustom;
	std::map<unsigned int, std::string>				allError;

	if (!this->exist_file(pathCgi) || !this->permForOpen(pathCgi))
	{
		req.setStatus("502");
		this->error_page(req);
		return (false);
	}
	return (true);
}

bool	TreatRequest::is_dynamic( Parse_request const & req,
	std::string const & extension,
	std::map<std::string, std::string>::const_iterator & it ) const
{
	std::map<std::string, std::string>::const_iterator	end;
	end = it = this->_loc->second.getCgi().end();
	for (it = this->_loc->second.getCgi().begin() ; it != end ; ++it)
	{
		if (extension == it->first)
			return (true);
	}
	return (false);
}

bool	TreatRequest::openAndRead( std::string const & path,
	Parse_request & req )
{
	std::ifstream ifs;
	std::map<std::string, std::string>::const_iterator	it_cgi;
	std::string	extension;

	if (!exist_file(path))
		return (false);

	ifs.open(path.c_str(), std::ifstream::in);
	if (!(ifs.is_open()))
		return (false);

	extension =	&path[path.rfind('/')];
	extension = &path[path.rfind('.')];
	this->_cgi = this->is_dynamic(req, extension, it_cgi);

	//std::cout << _loc->second << std::endl;
	//std::cout << "_cgi\t=\t" << _cgi << std::endl;

	if (this->_cgi)
	{
		if (this->check502(it_cgi->second, req))
			this->readDynamicFile(path, it_cgi->second, req);
		else
			return (true);
	}
	else if ((!this->_cgi && req.get_request("Method") == "GET"))
		this->readStaticFile(ifs);
	else
	{
		req.setStatus("405");
		this->error_page(req);
		return (true);
	}
	this->cpyInfo(extension, path, req);
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
	if (locName[i] || (i > 0 && path[i]
		&& path[i - 1] != '/' && path[i] != '/'))
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

	end = this->_loc->second.getIndex().end();
	for (it = this->_loc->second.getIndex().begin() ; it != end ; ++it)
	{
		tmp = path + *it;
		if (this->openAndRead(tmp, req))
			return (true);
		this->_cgi = false;
	}
	return (false);
}

void	TreatRequest::generateAutoIndex( Parse_request & req,
	std::string const & path )
{
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

void	TreatRequest::error_page( Parse_request & req )
{
	std::map<std::string, Server>::const_iterator	locTmp;
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
		locTmp = this->_loc;
		this->_loc = this->selectLocation(allError[code],
			this->_conf[this->_i_conf].getLocation());
		path = this->_loc->second.getRoot() + allError[code];
		if (this->openAndRead(path, req))
			find_custom = true;
		else
			this->_loc = locTmp;
	}
	if (!find_custom)
		this->force_open(req);
}

void	TreatRequest::redirect( Parse_request & req, std::string const & path )
{
	this->_location = "http://"
		+ req.get_request("Host-uncut-comme-les-casquettes")
		+ ":" + sizet_to_string(this->_eng->GetAccessPort())
		+ req.get_request("Path") + "/";
	req.setStatus("301");
	this->error_page(req);
}

bool	TreatRequest::check_access( Parse_request & req, std::string path )
{
	std::string	testPath;
	size_t		size_parced;
	bool		is_file;

	is_file = false;
	while (path.length())
	{
		size_parced = path.find('/');
		if (size_parced == std::string::npos)
		{
			is_file = true;
			size_parced = path.length();
		}
		else
			++size_parced;
		testPath.insert(testPath.end(), path.begin(),
			path.begin() + size_parced);
		if (is_file && (this->exist_dir(testPath)
			|| this->exist_dir(testPath + "/")))
		{
			if (!this->permForOpen(testPath))
				req.setStatus("403");
			else
				this->redirect(req, testPath);
			return (false);
		}
		else if (!this->exist(testPath))
		{
			req.setStatus("404");
			return (false);
		}
		else if (!this->permForOpen(testPath))
		{
			req.setStatus("403");
			return (false);
		}
		path.erase(0, size_parced);
	}
	return (true);
}

void	TreatRequest::exec_root( Parse_request & req,
	std::string const & path )
{
	if (!this->check_access(req, path))
		this->error_page(req);
	else if (path[path.length() - 1] == '/')
	{
		if (!this->search_index(req, path))
			this->generateAutoIndex(req, path);
	}
	else
		this->openAndRead(path, req);
}

void	TreatRequest::parce_end_path( std::string & src )
{
	for (std::string::iterator it = src.end()-1 ; it != src.begin() ; --it)
	{
		if (*it == '/' && *(it - 1) == '/')
		{
			src.erase(it);
			it = src.end();
		}
		else
			return ;
	}
}


bool	TreatRequest::check_access_delete( Parse_request & req,
	std::string path )
{
	std::string	testPath;
	size_t		size_parced;

	this->parce_end_path(path);
	while (path.length())
	{
		size_parced = path.find('/');
		if (size_parced == std::string::npos
			|| (size_parced + 1) == path.length() )
			return (true);
		++size_parced;
		testPath.insert(testPath.end(), path.begin(),
			path.begin() + size_parced);
		if (this->exist_dir(testPath) && !this->permForOpen(testPath))
		{
			req.setStatus("403");
			return (false);
		}
		else if (!this->exist_dir(testPath))
		{
			req.setStatus("404");
			return (false);
		}
		path.erase(0, size_parced);
	}

	return (true);
}

bool	TreatRequest::del( char const * path )
{
	struct stat sb;

	if (stat(path, &sb) != -1)
	{
		if (S_ISDIR(sb.st_mode))
		{
			if (rmdir(path) == -1)
				return (false);
		}
		else
		{
			if (unlink(path) == -1)
				return (false);
		}
	}
	else
		return (false);
	return (true);
}

void	TreatRequest::exec_delete( Parse_request & req,
	std::string const & path )
{
	if (this->check_access_delete(req, path))
	{
		if (!this->exist(path))
			req.setStatus("404");
		else if (!this->exist_file(path) && *(--(path.end())) != '/')
			req.setStatus("409");
		else if (this->exist_dir(path) && !this->permForOpen(path))
			req.setStatus("500");
		else if (del(path.c_str()))
			req.setStatus("204");
		else
			req.setStatus("500");
	}
	if (req.get_request("Status") != "204")
		this->error_page(req);
}

void	TreatRequest::exec( Parse_request & req, std::string const & method )
{
	std::string		path;
	std::string		path_alias;

	if (!this->exist(this->_loc->second.getRoot() + req.get_request("Path"))
		&& this->_loc->first != "/")
	{
		path_alias = req.get_request("Path");
		path_alias.erase(0, this->_loc->first.length());

		path = this->_loc->second.getRoot() + path_alias;
		if (method == "POST" || method == "GET")
			this->exec_root(req, path);
		else if (method == "DELETE")
			this->exec_delete(req, path);
	}
	else
	{
		path = this->_loc->second.getRoot() + req.get_request("Path");
		if (method == "POST" || method == "GET")
			this->exec_root(req, path);
		else if (method == "DELETE")
			this->exec_delete(req, path);
	}
}

void	TreatRequest::permMethod( Parse_request & req )
{
	std::string	method;
	std::set<std::string>::const_iterator	it;

	method = req.get_request("Method");
	it = this->_loc->second.getMethods().find(method);
	if (it == this->_loc->second.getMethods().end()
		|| (method != "GET"
		&& method != "POST"
		&& method != "DELETE"))
	{
		req.setStatus("405");
		this->error_page(req);
	}
	else
		this->exec(req, method);
}

std::string	TreatRequest::treat(Parse_request & req )
{
	// DISPLAY (TODO DELETE)
	//std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	//printMap(pol, "Tableau de merde");

	if (req.get_request("Status") == "400"
		|| req.get_request("Status") == "505") // TODO 505 test when merge
		force_open(req);
	else
	{
		this->_i_conf = this->selectConf(req);
		this->_loc = this->selectLocation(req.get_request("Path"),
			this->_conf[this->_i_conf].getLocation());

		if (req.get_request_body_size() > this->_loc->second.getMaxbody()
			&& this->_loc->second.getMaxbody() != 0)
			req.setStatus("413");
		if (req.get_request("Status") != "200")
			this->error_page(req);
		else
			this->permMethod(req);
	}

	Response	rep(req, *this);

	return (rep.getResponse());
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &	TreatRequest::getLocation( void ) const
{
	return (this->_location);
}

std::string const &	TreatRequest::getLastModif( void ) const
{
	return (this->_last_modif);
}

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
