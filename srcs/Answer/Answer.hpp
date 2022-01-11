/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Answer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:41:56 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/11 09:17:50 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANSWER_HPP
# define ANSWER_HPP

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include <cctype>
# include <fstream>
# include <exception>

class Answer
{

	public:

		Answer( unsigned int const & code, std::string const & page );
		Answer( Answer const & src );
		~Answer();

		Answer &		operator=( Answer const & rhs );

	private:
		Answer();

		void	writeRequestStatus( unsigned int const & code );
		void	writeDate( void );

		std::string	getHeader( void ) const;

		std::string _header;

};

std::ostream &			operator<<( std::ostream & o, Answer const & i );

#endif /* ********************************************************** ANSWER_H */
