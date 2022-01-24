/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreatRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:34:34 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/24 17:26:31 by tsannie          ###   ########.fr       */
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

		Server const *	_conf;

		std::string	printError( Parse_header const & req );

		//Parse_header _request;
};

std::ostream &			operator<<( std::ostream & o, TreatRequest const & i );

#endif /* **************************************************** TREATREQUEST_H */
