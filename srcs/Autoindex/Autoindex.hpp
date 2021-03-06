/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 13:52:00 by tsannie           #+#    #+#             */
/*   Updated: 2022/03/02 11:44:52 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include <iostream>
# include <map>
# include <string>
# include <cstring>
# include <dirent.h>
# include <sys/stat.h>
# include <sstream>


class Autoindex
{

	public:

		Autoindex(const char *root, std::string const & path);
		Autoindex( Autoindex const & src );
		~Autoindex();

		Autoindex &		operator=( Autoindex const & rhs );

		std::string	getPage( void ) const;

	private:

		Autoindex();

		std::string							_pathStr;
		std::map<std::string, std::string>	_href;

		void	setAllHref( DIR *dir, const char *root );
		void	insertAlign( std::string & path,
					time_t const & date, off_t const & size );

};

std::ostream &			operator<<( std::ostream & o, Autoindex const & i );

#endif /* ******************************************************* AUTOINDEX_H */
