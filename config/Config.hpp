/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/07 19:04:10 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <cctype>
# include <exception>
# include <fstream>
# include <vector>
# include <set>
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

std::vector< std::vector<std::string> >	sortInVec( std::string const & src );

#endif /* ********************************************************** CONFIG_H */
