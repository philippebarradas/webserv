/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/20 11:36:25 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include <cctype>
# include <exception>
# include <fstream>
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

		std::vector<Server>	getConfig() const;

	private:

		void	parsingAll( std::vector<
					std::vector<std::string> > const & src );

		std::vector<Server>		_config;
};

std::ostream &			operator<<( std::ostream & o, Config const & i );


#endif /* ********************************************************** CONFIG_H */