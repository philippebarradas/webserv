/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/07 14:46:38 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include <cctype>
# include <fstream>
# include <exception>
# include <vector>
# include <set>
# include <map>
# include "Server.hpp"
# include "utils.hpp"
class Server;

class Config
{

	public:

		Config();
		Config( Config const & src );
		Config( const char *file_name );
		~Config();

		Config &		operator=( Config const & rhs );

		std::vector<Server> const &	getConfig() const;

	private:

		void	parsingAll( std::vector<
					std::vector<std::string> > const & src );

		std::vector<Server>		_config;
};

std::ostream &			operator<<( std::ostream & o, Config const & i );


#endif /* ********************************************************** CONFIG_H */
