/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:34 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/25 16:27:30 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREATREQUEST_HPP
# define TREATREQUEST_HPP

# include <iostream>
# include "../Parse_header/parse_header.hpp"
# include "Response.hpp"
# include <string>

class	Parse_header;
class	Server;

class TreatRequest
{

	public:

		TreatRequest( std::vector<Server> const & conf, int const & access_port );
		TreatRequest( TreatRequest const & src );
		~TreatRequest();

		TreatRequest &		operator=( TreatRequest const & rhs );

		std::string	exec( Parse_header const & req );

	private:

		TreatRequest( void );

		 std::vector<Server>	_conf;

		size_t		selectConf( Parse_header const & req ) const;
		std::string	openAndRead( std::string const & path ) const;
		std::string	printError( Parse_header const & req,
			size_t const & i_conf ) const

		//Parse_header _request;
};

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i );

#endif /* **************************************************** TREATREQUEST_H */
