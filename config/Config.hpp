/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/13 15:44:03 by tsannie          ###   ########.fr       */
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
# include "utils.hpp"

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

		void	split_server( std::vector<
					std::vector<std::string> > const & src );


		std::vector<Server>		serv;
};

std::ostream &			operator<<( std::ostream & o, Config const & i );


#endif /* ********************************************************** CONFIG_H */
