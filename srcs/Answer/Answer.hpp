/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Answer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:41:56 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/10 16:41:58 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANSWER_HPP
# define ANSWER_HPP

# include <iostream>
# include <string>

class Answer
{

	public:

		Answer( unsigned int const & code, std::string const & page );
		Answer( Answer const & src );
		~Answer();

		Answer &		operator=( Answer const & rhs );

	private:
		Answer();

};

std::ostream &			operator<<( std::ostream & o, Answer const & i );

#endif /* ********************************************************** ANSWER_H */
