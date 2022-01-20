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

class Response
{

	public:

		Response( unsigned int const & code, std::string const & page );
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );

	private:
		Response();

		void	writeRequestStatus( unsigned int const & code );
		void	writeDate( void );

		std::string	getHeader( void ) const;

		std::string _header;

};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ********************************************************** RESPONSE_H */
