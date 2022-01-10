/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Answer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:40:33 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/10 16:53:48 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Answer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Answer::Answer( unsigned int const & code, std::string const & page )
{
	unsigned int accept[] = {200};
	std::string status[] = {"OK"};
}

Answer::Answer()
{
}

Answer::Answer( Answer const & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Answer::~Answer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Answer &				Answer::operator=( Answer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Answer const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
