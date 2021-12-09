/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:02 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/09 15:03:00 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <set>
# include "Location.hpp"
# include "Config.hpp"

class Server
{

	public:

		Server( Server const & src );
		Server( std::string const & src );
		~Server();

		std::set<std::string>				getName() const;
		std::set<std::string>				getIndex() const;
		std::string							getListen() const;
		std::string							getRoot() const;
		bool								getAutoindex() const;
		unsigned int						getMaxbody() const;
		std::map<unsigned int, std::string>	getError() const;

		Server &		operator=( Server const & rhs );

	private:

		Server();

		static bool	_alreadySetListen;
		static bool	_alreadySetRoot;
		static bool	_alreadySetAutoindex;
		static bool	_alreadySetMaxbody;

		void	setName( std::vector<std::string> const & src );
		void	setIndex( std::vector<std::string> const & src );
		void	setListen( std::vector<std::string> const & src );
		void	setRoot( std::vector<std::string> const & src );
		void	setAutoindex( std::vector<std::string> const & src );
		void	setMaxbody( std::vector<std::string> const & src );
		void	setError( std::vector<std::string> const & src );

		void	parsingAll( std::set< std::vector<std::string> > const & src );

		std::set<std::string>				_server_name;
		std::set<std::string>				_index;
		std::string							_listen;
		std::string							_root;
		bool								_autoindex;
		unsigned int						_maxbody;
		std::map<unsigned int, std::string>	_error;


};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
