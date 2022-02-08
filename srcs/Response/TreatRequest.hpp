/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:34 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/08 11:53:22 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREATREQUEST_HPP
# define TREATREQUEST_HPP

# include <iostream>
# include "../Parse_request/parse_request.hpp"
# include "Response.hpp"
# include <string>
# include <dirent.h>
# include <sys/stat.h>

class	Parse_request;
class	Server;

class TreatRequest
{

	public:

		TreatRequest( std::vector<Server> const & conf,
			int const & access_port );
		TreatRequest( TreatRequest const & src );
		~TreatRequest();

		TreatRequest &		operator=( TreatRequest const & rhs );

		std::string			treat( Parse_request const & req );

		std::string const &	getExtension( void ) const;
		std::string const &	getFile( void ) const;

	private:

		TreatRequest( void );

		std::vector<Server>	_conf;


		bool	is_dir( std::string const & path ) const;

		void	search_index( Parse_request const & req,
			std::string const & path );

		void	exec_root( Parse_request const & req);
		void	exec( Parse_request const & req);

		void	selectLocation(
			Parse_request const & req,
			std::map<std::string, Server> const & allLoc );
		size_t	similarity_point(std::string const & locName,
			std::string const & path) const;
		size_t		selectConf( Parse_request const & req ) const;


		void	readDynamicFile( std::string const & path );
		void	readStaticFile( std::string const & path );
		void	openAndRead( std::string const & path );
		void	cpyInfo( std::ifstream const & ifs, std::string const & path );


		std::string	_file;
		std::string	_extension;
		std::map<std::string, Server>::const_iterator	_loc;
};

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i );

#endif /* **************************************************** TREATREQUEST_H */
