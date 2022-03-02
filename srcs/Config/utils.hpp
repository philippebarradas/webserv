/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 15:27:37 by tsannie           #+#    #+#             */
/*   Updated: 2022/03/02 13:12:19 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "Config.hpp"

# define BRED		"\033[1;31m"
# define BGREEN		"\033[1;32m"
# define BYELLOW	"\033[1;33m"
# define BBLUE		"\033[1;34m"
# define BPURPLE	"\033[1;35m"
# define BCYAN		"\033[1;36m"
# define BWHITE		"\033[1;37m"
# define END		"\033[0m"

class SignalStop: public std::exception {
public:
	virtual const char * what() const throw() {
		return ("Sign");
	}
};

template <typename T>
inline void	printContainers( T const & ctn, std::ostream & o )
{
	typename	T::const_iterator	it, end;

	end = ctn.end();
	o << BWHITE "[ " BBLUE;
	for (it = ctn.begin() ; it != end ; ++it)
		o << BWHITE "\'" BBLUE << *it << BWHITE "\' " END;
	o << BWHITE  "]" END << std::endl;
}

inline void	printContainers( std::string const & ctn, std::ostream & o )
{
	std::string::const_iterator	it, end;

	end = ctn.end();

	o << BBLUE;
	for (it = ctn.begin() ; it != end ; ++it)
		o << *it;
	o << END << std::endl;
}

template <typename T1, typename T2>
inline void	printContainers( std::map<T1, T2> const & map, std::ostream & o )
{
	typename	std::map<T1, T2>::const_iterator	it, end;

	end = map.end();
	for (it = map.begin() ; it != end ; ++it)
		o << BWHITE "{\'" BRED << it->first << BWHITE "\' => \'" BBLUE
			<< it->second << BWHITE "\'} " END << std::endl;
}

template <typename T>
void	printType( T const &type, std::ostream & o, std::string const & name )
{
	if (!type.size())
		return ;

	if (name == "location")
	{
		o << std::endl << BWHITE;
		for (size_t y = 0 ; y < 30 ; ++y)
		{
			if (y == 15)
				o << BYELLOW << name << BWHITE;
			o << '-';
		}
		o << END << std::endl;
	}
	else
		o << BYELLOW << name << std::setw(16 - name.size()) << ": " END;

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
size_t			stost_size( size_t const & min, size_t const & max,
					std::string const & nb, std::string const & name );

std::string		sizet_to_string(size_t const & x);
size_t			hexa_to_size_brut(std::string nbr);


#endif
