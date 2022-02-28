/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:02 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/28 10:06:57 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <set>
# include "Config.hpp"
# include "../Server/Engine.hpp"

# define MAX_MAXBODY       9000000000000
# define MAXBODY_DEFAULT   10000000
# define AUTOINDEX_DEFAULT false
# define LISTEN_DEFAULT    "8000"
# define INDEX_DEFAULT     "index.html"
# define ROOT_DEFAULT      "www"

class Engine;

class Server
{

	public:

		Server( Server const & src );
		Server( std::string const & src );
		Server( void );

		~Server( void );

		std::set<std::string> const &				getName( void ) const;
		std::set<std::string> const &				getIndex( void ) const;
		std::set<std::string> const &				getMethods( void ) const;
		std::string const &							getListen( void ) const;
		std::string const &							getRoot( void ) const;
		bool const &								getAutoindex( void ) const;
		size_t const &								getMaxbody( void ) const;
		std::map<unsigned int, std::string> const &	getError( void ) const;
		std::map<std::string, std::string> const &	getCgi( void ) const;
		std::map<std::string, Server> const &		getLocation( void ) const;

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

		void	fillLocation( void );
		void	initServ( void );
		void	initLocation( void );
		void	parsingAll(
					std::vector< std::vector<std::string> > const & src );

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
		size_t								_maxbody;
		std::map<unsigned int, std::string>	_error;
		std::map<std::string, std::string>	_cgi;
		std::map<std::string, Server>		_location;



};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
