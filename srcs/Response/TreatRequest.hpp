/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:34 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/14 18:11:45 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREATREQUEST_HPP
# define TREATREQUEST_HPP

# include <iostream>
# include "../Parse_request/parse_request.hpp"
# include "../Autoindex/Autoindex.hpp"
# include "../Server/Engine.hpp"
# include "../Cgi/Cgi.hpp"
# include "Response.hpp"
# include <string>
# include <dirent.h>
# include <sys/stat.h>

# define DEFAULT_ROOT_ERROR "srcs/Config/default/error_page/"

class	Parse_request;
class	Engine;
class	Server;
class	Autoindex;

class TreatRequest
{

	public:

		TreatRequest( std::vector<Server> const & conf,
			Engine const & eng );
		TreatRequest( TreatRequest const & src );
		~TreatRequest();

		TreatRequest &		operator=( TreatRequest const & rhs );

		std::string			treat( Parse_request & req );

		std::string const &	getLocation( void ) const;
		std::string const &	getLastModif( void ) const;
		std::string const &	getExtension( void ) const;
		std::string const &	getFile( void ) const;
		std::string const &	getType_Cgi( void ) const;
		bool const &		getIs_Cgi( void ) const;

	private:

		TreatRequest( void );

		std::vector<Server>								_conf;
		std::map<std::string, Server>::const_iterator	_loc;
		Engine const *									_eng;

		std::string	_file;
		std::string	_location;
		std::string	_type_cgi;
		std::string	_last_modif;
		std::string	_extension;
		bool		_cgi;
		size_t		_i_conf;


		bool	is_dir( std::string const & path ) const;

		bool	search_index( Parse_request & req,
			std::string const & path );

		//bool	exist( std::string const & path, Parse_request & req) const;
		bool	check_access( Parse_request & req, std::string path );
		bool	exist_file( std::string const & path) const;
		bool	exist_dir( std::string const & root) const;
		bool	exist( std::string const & root) const;
		bool	permForOpen( std::string const & path ) const;
		void	permMethod( Parse_request & req );
		void	redirect( Parse_request & req, std::string const & path );
		void	exec_root( Parse_request & req, std::string const & path );
		void	exec( Parse_request & req );

		std::map<std::string, Server>::const_iterator	selectLocation(
			std::string const &	path,
			std::map<std::string, Server> const & allLoc );
		size_t	similarity_point(std::string const & locName,
			std::string const & path ) const;
		size_t	selectConf( Parse_request const & req ) const;

		void	generateAutoIndex( Parse_request & req,
			std::string const & path );

		void	readStaticFile( std::string const & path, std::ifstream & ifs );
		void	readDynamicFile( std::string const & path,
			std::string const & pathCgi,
			Parse_request const & req );

		void	cpyInfo( std::string const & extension,
			std::string const & path, Parse_request const & req );
		bool	openAndRead( std::string const & path,
			Parse_request & req, bool const & isError );

		void	error_page( Parse_request & req );

};

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i );

#endif /* **************************************************** TREATREQUEST_H */
