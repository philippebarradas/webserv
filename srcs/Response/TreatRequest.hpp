/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:34 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/07 14:13:05 by tsannie          ###   ########.fr       */
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

		std::string	treat( Parse_request const & req );

	private:

		TreatRequest( void );

		 std::vector<Server>	_conf;


		bool	is_dir( std::string const & path ) const;

		void	exec( Parse_request const & req,
			std::map<std::string, Server>::const_iterator const & loc ) const;
		void	exec_root( Parse_request const & req,
			std::map<std::string, Server>::const_iterator const & loc ) const;

		void	selectLocation(
			std::map<std::string, Server>::const_iterator & loc,
			Parse_request const & req,
			std::map<std::string, Server> const & allLoc ) const;
		size_t	similarity_point(std::string const & locName,
			std::string const & path) const;
		size_t		selectConf( Parse_request const & req ) const;
		std::string	openAndRead( std::string const & path ) const;
		std::string	printError( Parse_request const & req,
			size_t const & i_conf ) const;

		//Parse_request _request;
};

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i );

#endif /* **************************************************** TREATREQUEST_H */
