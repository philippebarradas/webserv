/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/09 21:44:41 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <cctype>
# include <exception>
# include <fstream>
# include <vector>
# include <set>
# include <map>
# include "Server.hpp"
# include "Location.hpp"

class Server;

class Config
{

	public:

		Config( Config const & src );
		Config( std::string const & file_name );
		~Config();

		Config &		operator=( Config const & rhs );

	private:
		Config();

		void	split_server( std::string & fileStr );

		std::vector<Server>		serv;
};

std::ostream &			operator<<( std::ostream & o, Config const & i );

std::set< std::vector<std::string> >	sortInVec( std::string const & src );

void			checkRedefinition( bool const & toCheck,
					std::string const & name );
void			checkNbArg( size_t const & toCheck, size_t const & size,
					std::string const & name );
void			checkNbArgMin( size_t const & toCheck, size_t const & size,
					std::string const & name );
unsigned int	stoui_size( unsigned int const & min, unsigned int const & max,
					std::string const & nb, std::string const & name );
void			checkNotValidDirective( std::vector<std::string> const & src );
#endif /* ********************************************************** CONFIG_H */
