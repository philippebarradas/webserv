/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 10:11:41 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/17 14:24:07 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Autoindex::Autoindex(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (!dir)
		throw InvalidPath();
	this->_pathStr = std::string(path);
	this->setAllHref(dir);
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
		//this->_path = rhs._path;
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

	page = "<html>\n<head><title>Index of " + this->_pathStr
		+ "</title></head>\n"
		+ "<body bgcolor=\"white\">\n"
		+ "<h1>Index of " + this->_pathStr + "</h1><hr><pre>";

	return (page);
}

void		Autoindex::insertAlign( std::string const & path,
	time_t const & date, off_t const & size )
{
	struct tm	*timeinfo;
	char		buffer[200];

	timeinfo = localtime(&date);
	strftime(buffer, 200, "%d-%b-%G %T",timeinfo);
	std::cout << path << std::endl;
	std::cout << buffer << std::endl;
	std::cout << size << std::endl;
	std::cout << std::endl;
}

void		Autoindex::setAllHref( DIR *dir )
{
	struct dirent	*pDirent;
	struct stat		attr;
	std::string		pathfile;

	while ((pDirent = readdir(dir)))
	{
		if (pDirent->d_name[0] != '.')
		{
			pathfile = this->_pathStr + '/' + pDirent->d_name;
			std::cout << pathfile << std::endl;

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

		//std::cout << "pDirent->d_name\t=\t" << pDirent->d_name << std::endl;
		//std::cout << "pDirent->d_reclen\t=\t" << attr.st_size << std::endl;
		//std::cout << "this->path\t=\t" << this->_path << std::endl;
	}
	std::cout << std::endl;
}

char const *Autoindex::InvalidPath::what() const throw()
{
	return "[Error] Cannot open directory.";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
