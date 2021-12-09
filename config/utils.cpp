/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 19:02:55 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/09 16:40:16 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

unsigned int	stoui_size( unsigned int const & min, unsigned int const & max,
	std::string const & nb, std::string const & name )
{
	std::string::const_iterator	it, end;
	std::stringstream			conv, conv_max, conv_min;
	unsigned int				ret;

	end = nb.end();
	for (it = nb.begin() ; it != end && std::isdigit(*it) ; ++it) {}

	conv << nb;
	conv >> ret;

	if (nb.size() > 9 || it != end)
	{
		std::string thr("[Error] invalid arguments in \'");
		thr += name + "\' (\'" + nb + "\' is not un valid number).";
		throw std::invalid_argument(thr);
	}
	else if (ret > max || ret < min)
	{
		conv_max << max;
		conv_min << min;
		std::string thr("[Error] invalid arguments in \'");
		thr += name + "\' (\'" + nb +"\' is too high. Must be between "
			+ conv_min.str() + " and " + conv_max.str() + ").";

		throw std::invalid_argument(thr);
	}
	return (ret);
}

void	checkRedefinition( bool const & toCheck, std::string const & name )
{
	if (toCheck)
	{
		std::string thr("[Error] \'");
		thr += name;
		thr += "\' directive is duplicate.";
		throw std::invalid_argument(thr);
	}
}

void	checkNbArg( size_t const & toCheck, size_t const & size,
	std::string const & name )
{
	if (toCheck != size)
	{
		std::string thr("[Error] invalid number of arguments in \'");
		thr += name;
		thr += "\'.";
		throw std::invalid_argument(thr);
	}
}

std::string	nextWord( std::string const & src, size_t & i )
{
	std::string	ret;

	for (; isspace(src[i]) && src[i] ; ++i) {}
	for (; !isspace(src[i]) && src[i] != ';' && src[i] != '}' && src[i] ; ++i)
	{
		if (src[i] == '{')
		{
			for (; src[i] != '}' && src[i] ; ++i)
				ret += src[i];
			ret += src[i];
			--i;
		}
		else
			ret += src[i];
	}

	return (ret);
}

std::set< std::vector<std::string> >	sortInVec( std::string const & src )
{
	std::set< std::vector<std::string> >			ret;
	std::pair<std::set<
		std::vector<std::string> >::iterator, bool>	itret;
	std::vector<std::string>						tmp;
	std::string	nxt;
	size_t	i = 0;

	while (src[i])
	{
		nxt = nextWord(src, i);
		tmp.push_back(nxt);
		if (src[i] == ';' || src[i] == '}')
		{
			itret = ret.insert(tmp);
			if (itret.second == false)
				checkRedefinition(true, *(tmp.begin()));
			tmp.clear();
			++i;
		}
	}

	// print content (to delete)
	std::set< std::vector<std::string> >::const_iterator	it, end;
	std::vector<std::string>::const_iterator	sit, send;
	int j = 0;

	end = ret.end();
	for (it = ret.begin() ; it != end ; ++it)
	{
		std::cout << ++j << std::endl;
		send = (*it).end();
		for (sit = (*it).begin() ; sit != send ; ++sit)
			std::cout << *sit << "\n";
		std::cout << "\n";
	}
	return (ret);
}
