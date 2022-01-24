/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:41:56 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/11 09:17:50 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include <cctype>
# include <fstream>
# include <exception>
# include "../Parse_header/parse_header.hpp"

class	Parse_header;
class	Server;

class Response
{

	public:

		Response( Parse_header const & req, std::string const & page, std::string const & extension );
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );

		std::string	getHeader( void ) const;

	private:

		Response();

		void	writeRequestStatus( std::string const & code );
		void	writeRequestStatus( unsigned int const & code );
		void	writeDate( void );
		void	writeType( std::string const & extension );
		void	writeLenght( std::string const & page );


		std::string _header;

};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ********************************************************** RESPONSE_H */
