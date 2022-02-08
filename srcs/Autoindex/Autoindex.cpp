/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 10:11:41 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/08 13:52:40 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Autoindex::Autoindex(const char *root, std::string const & path): _pathStr(path)
{
	DIR	*dir;

	dir = opendir(root);
	if (!dir)
		throw InvalidRoot();
	this->setAllHref(dir, root);
	closedir(dir);
}

Autoindex::Autoindex()
{
}

Autoindex::Autoindex( const Autoindex & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Autoindex::~Autoindex()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Autoindex &				Autoindex::operator=( Autoindex const & rhs )
{
	if ( this != &rhs )
	{
		this->_href = rhs._href;
		this->_pathStr = rhs._pathStr;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Autoindex const & i )
{
	o << i.getPage();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	Autoindex::getPage( void ) const
{
	std::string	page;
	std::map<std::string, std::string>::const_iterator it, end;

	page = "<html>\n<head><title>Index of " + this->_pathStr
		+ "</title></head>\n"
		+ "<body bgcolor=\"white\">\n"
		+ "<h1>Index of " + this->_pathStr + "</h1><hr><pre>";

	end = this->_href.end();
	for (it = this->_href.begin() ; it != end ; ++it)
	{
		if (it->first[it->first.length() - 1] == '/')
		{
			page += "<a href=\"" + it->first + "\">" + it->first + "</a>"
				+ it->second + "\n";
		}
	}
	end = this->_href.end();
	for (it = this->_href.begin() ; it != end ; ++it)
	{
		if (it->first[it->first.length() - 1] != '/')
		{
			page += "<a href=\"" + it->first + "\">" + it->first + "</a>"
				+ it->second + "\n";
		}
	}

	page += "</pre><hr></body>\n</html>\n";

	return (page);
}

void		Autoindex::insertAlign( std::string const & path,
	time_t const & date, off_t const & size )
{
	struct tm			*timeinfo;
	char				buffer[30];
	std::string			log, log_size, log_date;

	bzero(&buffer, 30);
	timeinfo = localtime(&date);
	strftime(buffer, 30, "%d-%b-%G %T",timeinfo);

	if (size >= 0)
	{
		std::stringstream	conv;
		conv << size;
		log_size = conv.str();
	}
	else
		log_size = "-";
	log_date = std::string(buffer);
	log_date.erase(log_date.length() - 3, 3);

	log = std::string(51 - path.length(), ' ') + log_date
		+ std::string(20 - log_size.length(), ' ') + log_size;

	this->_href.insert(std::make_pair(path, log));
}

void		Autoindex::setAllHref( DIR *dir, const char *root )
{
	struct dirent	*pDirent;
	struct stat		attr;
	std::string		pathfile;

	while ((pDirent = readdir(dir)))
	{
		if (pDirent->d_name[0] != '.')
		{
			pathfile = std::string(root) + '/' + pDirent->d_name;
			stat(pathfile.c_str(), &attr);

			if (S_ISDIR(attr.st_mode))
				this->insertAlign(std::string(pDirent->d_name) + "/",
					attr.st_mtim.tv_sec, -1);
			else
				this->insertAlign(std::string(pDirent->d_name),
					attr.st_mtim.tv_sec, attr.st_size);
		}
		else if (strcmp(pDirent->d_name, "..") == 0)
			this->_href.insert(std::make_pair("../", ""));
	}
}

char const *Autoindex::InvalidRoot::what() const throw()
{
	return "[Error] Cannot open directory.";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
