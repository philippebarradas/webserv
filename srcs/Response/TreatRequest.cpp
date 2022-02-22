/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:30 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/22 18:41:19 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TreatRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

// TODO: 405 bad extension text plain

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

bool	TreatRequest::check_precondition( Parse_request const & req, struct tm const & timefile ) const
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

	if (req.get_request("Status") == "200")
	{
		stat(path.c_str(), &result);
		timeinfo = localtime(&result.st_ctim.tv_sec);
		strftime(time_modified_file, 50, "%a, %d %b %Y %T %Z", timeinfo);
		if (!check_precondition(req, *timeinfo))
		{
			//std::cout << "ERROR 412" << std::endl;
			this->_file.clear();
			req.setStatus("412");
			this->error_page(req);
			return ;
		}
		this->_last_modif = std::string(time_modified_file);

		//std::cout << "_last_modif\t=\t" << _last_modif << std::endl;
	}
	//std::cout << "_last_modif\t=\t" << _last_modif << std::endl;
	this->_extension = extension;
}

void	TreatRequest::readStaticFile( std::ifstream & ifs )
{
	std::cout << "--------------------\nSTATIC READ" << std::endl;
	std::string	line;

	while (std::getline(ifs, line))
		this->_file += line + "\n";
	ifs.close();
}

void	TreatRequest::readDynamicFile( std::string const & path, std::string const & pathCgi,
	Parse_request const & req )
{

	std::cout << "--------------------\nDYNAMIC READ" << std::endl;
	//std::cout << "pathCgi\t=\t" << pathCgi << std::endl;
	Cgi	obj_cgi(this->_loc->second.getRoot(), path, pathCgi, req, *this->_eng);

	obj_cgi.exec_cgi(obj_cgi.create_argv(path),
		obj_cgi.convert_env(obj_cgi.getEnv()), req);

	this->_file = obj_cgi.getSend_content();

	this->_type_cgi = obj_cgi.getType_Cgi();
}

// TODO 505

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
	//std::cout << "test perm = ";

	if (sbuf.st_mode & S_IRUSR)
	{
		//std::cout << "readable" << std::endl;
		return (true);
	}
	else
	{
		//std::cout << "not readable" << std::endl;
		return (false);
	}
	return (true);
	// true = perm ok / false = no perm(403)
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
	// true = exist  / false = no exist
	struct stat	sbuf_dir;

	if (stat(root.c_str(), &sbuf_dir) == -1)
		return (false);
	return (S_ISDIR(sbuf_dir.st_mode)); // dossier
}

bool	TreatRequest::exist( std::string const & root) const
{
	//std::cout << "test exist for:" << root << std::endl;
	if (this->exist_dir(root) || this->exist_file(root))
		return (true);
	return (false);
}

bool	TreatRequest::check502( std::string const & pathCgi, Parse_request & req )
{
	std::string	pathcustom;
	std::map<unsigned int, std::string>				allError;

	//std::cout << "CHECK 502" << std::endl;

	if (!this->exist_file(pathCgi) || !this->permForOpen(pathCgi))
	{
		//std::cout << "SEARCH CUSTOM 502" << std::endl;
		req.setStatus("502");
		//std::cout << "HELLO" << std::endl;
		this->error_page(req);
		return (false);
	}
	return (true);
}


// TODO NORM URGENT
bool	TreatRequest::openAndRead( std::string const & path,
	Parse_request & req, bool const & isError )
{
	std::ifstream ifs;
	std::map<std::string, std::string>::const_iterator	it, end;
	std::string	extension;

	if (!exist_file(path))// &&
		return (false);

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
				this->_cgi = true;		// TODO check plusieurs def de .php dans la confi
				break;
			}
		}
	}
	//std::cout << "path\t=\t" << path << std::endl;
	//std::cout << "this->_cgi\t=\t" << this->_cgi << std::endl;

	if (this->_cgi)
	{
		if (this->check502(it->second, req))
			this->readDynamicFile(path, it->second, req);
		else
			return (false);
	}
	else if ((!this->_cgi && req.get_request("Method") == "GET") || isError)
		this->readStaticFile(ifs);
	else
	{
		//std::cout << "ERROR 405" << std::endl;
		req.setStatus("405");
		this->error_page(req);
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

bool	TreatRequest::is_dir( std::string const & path ) const		// to delete ??
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
		if (this->openAndRead(tmp, req, false))
			return (true);
	}
	return (false);
}

void	TreatRequest::generateAutoIndex( Parse_request & req,
	std::string const & path )
{
	//std::cout << this->_loc->second.getAutoindex() << std::endl;

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
		this->_loc = this->selectLocation(allError[code], this->_conf[this->_i_conf].getLocation());

		//std::cout << "newloc\t=\t" << this->_loc->first << std::endl;

		path = this->_loc->second.getRoot() + allError[code];

		//std::cout << "locTmp->first\t=\t" << locTmp->first << std::endl;
		//std::cout << "pathERR\t=\t" << path << std::endl;
		//std::cout << std::endl;

		if (this->openAndRead(path, req, true))
			find_custom = true;
		else
			this->_loc = locTmp;
	}
	//std::cout << "find_custom\t=\t" << find_custom << std::endl;
	//std::cout << "_file\t=\t" << _file << std::endl;
	if (!find_custom)
		this->force_open(req);
	//std::cout << "BYE" << std::endl;
}

void	TreatRequest::redirect( Parse_request & req, std::string const & path )
{
	this->_location = "http://"
		+ req.get_request("Host-uncut-comme-les-casquettes")
		+ ":" + sizet_to_string(this->_eng->GetAccessPort())
		+ req.get_request("Path") + "/";
	//std::cout << "_location\t=\t" << _location << std::endl;
	req.setStatus("301");
	this->error_page(req);
}

// err TODO 406 ?? 408 ?? 411 ?? 413 ??

bool	TreatRequest::check_access( Parse_request & req, std::string path )
{
	std::string	testPath;
	int			size_parced;
	bool		is_file;		// theorie is file but possible to be a dir without '/'

//	std::cout << "hello:" << path << std::endl;
//	std::cout << "this->exist(\"/tmp/test\") = " << this->exist("/tmp/test") <<  std::endl;

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
		testPath.insert(testPath.end(), path.begin(), path.begin() + size_parced);
		//std::cout << "testPath\t=\t" << testPath << std::endl;
		//std::cout << "this->exist(testPath)\t=\t" << this->exist(testPath) << std::endl;
		//std::cout << "this->exist(testPath)\t=\t" << this->exist(testPath + "/") << std::endl;
		//std::cout << "this->exist(testPath)\t=\t" << this->exist(testPath) << std::endl;
		//std::cout << "testPath:\t=\t" << testPath << std::endl;
		//std::cout << "this->exist_dir(testPath + \'/\')\t=\t" << this->exist_dir(testPath + '/') << std::endl;
		//std::cout << "this->exist_dir(testPath)\t=\t" << this->exist_dir(testPath) << std::endl;
		if (is_file && (this->exist_dir(testPath) || this->exist_dir(testPath + "/"))) // == the last parced part
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
			//std::cout << "404" << std::endl;
			return (false);
		}
		else if (!this->permForOpen(testPath))
		{
			//std::cout << "403 sauce" << std::endl;
			req.setStatus("403");
			return (false);
		}
		path.erase(0, size_parced);
		//std::cout << "size_parced\t=\t" << size_parced << std::endl;
		//std::cout << "testPath\t=\t" << testPath << std::endl;
		//std::cout << "path\t\t=\t" << path << std::endl;
		//std::cout << std::endl << std::endl << std::endl;
		//std::cout << testPath << std::endl;
	}
	//std::cout << "path.find('/')\t=\t" << path.find('/') << std::endl;
	//std::cout << "size_parced\t=\t" << size_parced << std::endl;
	//std::cout << "path.length()\t=\t" << path.length() << std::endl;
	//std::cout << "LEAVE PERM" << std::endl;
	return (true);
}

void	TreatRequest::exec_root( Parse_request & req, std::string const & path )
{
	//std::string	path = this->_loc->second.getRoot() + req.get_request("Path");
	//std::cout << "this->permForOpen(path\t=\t" << this->permForOpen(path) << std::endl;
	//std::cout << "this->exist(path)\t=\t" << this->exist(path) << std::endl;
	//std::cout << "pathDEBatarD\t=\t" << path << std::endl;
	if (!this->check_access(req, path))
	{
		//std::cout << RED "NO PERM" END << std::endl;
		this->error_page(req);
	}
	else if (path[path.length() - 1] == '/')
	{
		//std::cout << GREEN "OK PERM DIR" END << std::endl;
		//std::cout << "pathDEBatarD\t=\t" << path << std::endl;
		if (!this->search_index(req, path))
			this->generateAutoIndex(req, path);
	}
	else
	{
		//std::cout << GREEN "OK PERM FILE" END << std::endl;
		//this->_file += "ADDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD1\n";
		this->openAndRead(path, req, false);
		//this->_file += "ADDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD2\n";
	}
}

void	TreatRequest::exec( Parse_request & req )
{
	std::string		path;
	std::string		path_alias;

	//std::cout << "fake path:" <<  this->_loc->second.getRoot() + req.get_request("Path") <<  std::endl;
	//std::cout << "req.get_request(\"Path\")\t=\t" << req.get_request("Path") << std::endl;

	if (!this->exist(this->_loc->second.getRoot() + req.get_request("Path"))
		&& this->_loc->first != "/")					// TODO JUST OPEN OR WITH PERMISSION ??
	{
		//std::cout << "_loc->first\t=\t" << _loc->first << std::endl;
		//std::cout << "ALIAS METHOD" << std::endl;

		path_alias = req.get_request("Path");
		path_alias.erase(0, this->_loc->first.length());

		//std::cout << "path\t=\t" << path << std::endl;
		//std::cout << "path_alias\t=\t" << path_alias << std::endl;

		path = this->_loc->second.getRoot() + path_alias;
		exec_root(req, path);
	}
	else
	{
		//std::cout << "ROOT METHOD" << std::endl;
		path = this->_loc->second.getRoot() + req.get_request("Path");
		exec_root(req, path);
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
	if (it == this->_loc->second.getMethods().end()
		|| (cmd != "GET"
		&& cmd != "POST"
		&& cmd != "DELETE"))
	{
		//std::cout << "NO METHOD" << std::endl;
		req.setStatus("405");
		this->error_page(req);
	}
	else
	{
		if (cmd == "POST" || cmd == "GET")
			this->exec(req);
		//else
		//	TODO DELETE
	}
}

std::string	TreatRequest::treat( Parse_request & req )
{
	// DISPLAY (TO DELETE)
	std::map<std::string, std::string> pol = req.getBigMegaSuperTab();
	//printMap(pol, "Tableau de merde");

	if (req.get_request("Status") == "400")
		force_open(req);
	else
	{
		this->_i_conf = this->selectConf(req);
		//std::cout << "i_conf\t=\t" << this->_i_conf << std::endl;
		this->_loc = this->selectLocation(req.get_request("Path"), this->_conf[this->_i_conf].getLocation());
		//std::cout << "location\t=\t" << _loc->first << std::endl
			//<< _loc->second << std::endl;

		if (req.get_request_body_size() > this->_loc->second.getMaxbody()
			&& this->_loc->second.getMaxbody() != 0)
			req.setStatus("413");
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
