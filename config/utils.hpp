/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 15:27:37 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/13 18:51:37 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "Config.hpp"

template <typename T>
inline void	printContainers( T const & ctn, std::ostream & o )
{
	typename	T::const_iterator	it, end;

	o << "[ ";
	end = ctn.end();
	for (it = ctn.begin() ; it != end ; ++it)
		o << "\'" << *it << "\' ";
	o << "]" << std::endl;
}

inline void	printContainers( std::string const & ctn, std::ostream & o )
{
	std::string::const_iterator	it, end;

	o << "[ ";
	end = ctn.end();
	for (it = ctn.begin() ; it != end ; ++it)
		o << *it;
	o << " ]" << std::endl;
}

template <typename T1, typename T2>
inline void	printContainers( std::map<T1, T2> const & map, std::ostream & o )
{
	typename	std::map<T1, T2>::const_iterator	it, end;

	o << "[ ";
	end = map.end();
	for (it = map.begin() ; it != end ; ++it)
		o << "{\'" << it->first << "\' => \'" << it->second << "\'} ";
	o << "]" << std::endl;
}

template <typename T>
void	printType( T const &type, std::ostream & o, std::string const & name )
{
	if (!type.size())
		return ;
	o << name << std::setw(12 - name.size()) << ": ";
	printContainers(type, o);
}

std::vector< std::vector<std::string> >	sortInVec( std::string const & src );

void			checkNotValidDirective( std::vector<std::string> const & src );
void			checkRedefinition( bool const & toCheck,
					std::string const & name );
void			checkNbArg( size_t const & toCheck, size_t const & size,
					std::string const & name );
void			checkNbArgMin( size_t const & toCheck, size_t const & size,
					std::string const & name );
unsigned int	stoui_size( unsigned int const & min, unsigned int const & max,
					std::string const & nb, std::string const & name );



#endif
