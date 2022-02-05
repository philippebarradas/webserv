/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:02 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/05 17:06:31 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <set>
# include "Config.hpp"
# include "../Server/Engine.hpp"

class Engine;

class Server
{

	public:

		Server( Server const & src );
		Server( std::string const & src );
		Server( void );

		~Server( void );

		std::set<std::string>				getName( void ) const;
		std::set<std::string>				getIndex( void ) const;
		std::set<std::string>				getMethods( void ) const;
		std::string							getListen( void ) const;
		std::string							getRoot( void ) const;
		bool								getAutoindex( void ) const;
		unsigned int						getMaxbody( void ) const;
		std::map<unsigned int, std::string>	getError( void ) const;
		std::map<std::string, std::string>	getCgi( void ) const;
		std::map<std::string, Server>		getLocation( void ) const;

		Server &		operator=( Server const & rhs );

	private:

		Server( std::vector< std::vector<std::string> > const & src );

		void	setName( std::vector<std::string> const & src );
		void	setIndex( std::vector<std::string> const & src );
		void	setMethods( std::vector<std::string> const & src );
		void	setListen( std::vector<std::string> const & src );
		void	setRoot( std::vector<std::string> const & src );
		void	setAutoindex( std::vector<std::string> const & src );
		void	setMaxbody( std::vector<std::string> const & src );
		void	setError( std::vector<std::string> const & src );
		void	setCgi( std::vector<std::string> const & src );
		void	setLocation( std::vector<std::string> const & src );

		void	parsingAll( std::vector< std::vector<std::string> > const & src );
		void	fillLocation( void );
		void	initServ( void );
		void	initLocation( void );

		bool	_alreadySetIndex;
		bool	_alreadySetMethods;
		bool	_alreadySetListen;
		bool	_alreadySetRoot;
		bool	_alreadySetAutoindex;
		bool	_alreadySetMaxbody;
		bool	_alreadySetError;
		bool	_alreadySetCgi;

		std::set<std::string>				_name;
		std::set<std::string>				_index;
		std::set<std::string>				_methods;
		std::string							_listen;
		std::string							_root;
		bool								_autoindex;
		unsigned int						_maxbody;
		std::map<unsigned int, std::string>	_error;
		std::map<std::string, std::string>	_cgi;
		std::map<std::string, Server>		_location;



};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
