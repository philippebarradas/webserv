/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:02:02 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/21 18:07:28 by dodjian          ###   ########.fr       */
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
		~Server();

		std::set<std::string>				getName() const;
		std::set<std::string>				getIndex() const;
		std::set<std::string>				getMethods() const;
		std::string							getListen() const;
		std::string							getRoot() const;
		bool								getAutoindex() const;
		unsigned int						getMaxbody() const;
		std::map<unsigned int, std::string>	getError() const;
		std::map<std::string, std::string>	getCgi() const;
		std::map<std::string, Server>		getLocation() const;

		Server &		operator=( Server const & rhs );

	private:

		Server();
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
		void	initServ();

		bool	_alreadySetMethods;
		bool	_alreadySetListen;
		bool	_alreadySetRoot;
		bool	_alreadySetAutoindex;
		bool	_alreadySetMaxbody;

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
