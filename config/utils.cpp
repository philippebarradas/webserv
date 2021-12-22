/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 19:02:55 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/13 17:48:05 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

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
		thr += name + "\' (\'" + nb +"\' code is not valid. Must be between "
			+ conv_min.str() + " and " + conv_max.str() + ").";

		throw std::invalid_argument(thr);
	}
	return (ret);
}

void	checkNotValidDirective( std::vector<std::string> const & src )
{
	checkNbArgMin(src.size(), 2, src[0]);
	std::vector<std::string>::const_iterator	it, end;

	end = src.end();
	for (it = src.begin() + 1 ; it != end ; ++it)
	{
		if ((src[0] != "location" && src[0] != "server")
			&& (((*it).find('{') != std::string::npos)
			|| ((*it).find('}') != std::string::npos)))
		{
			std::string thr("[Error] unexpected \'");
			thr += (((*it).find('{') == std::string::npos) ? "}" : "{");
			thr += "\'.";
			throw std::invalid_argument(thr);
		}
	}

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

void	checkNbArgMin( size_t const & toCheck, size_t const & size,
	std::string const & name )
{
	if (toCheck < size)
	{
		std::string thr("[Error] invalid number of arguments in \'");
		thr += name;
		thr += "\'.";
		throw std::invalid_argument(thr);
	}
}

std::string	parceBrackets( std::string const & src, size_t & i, std::string & ret )
{
	size_t brackets_open;

	if (ret.size() != 0)
		return (ret);

	ret += src[i];
	brackets_open = 1;
	for (i = i + 1 ; brackets_open && src[i] ; ++i)
	{
		if (src[i] == '{')
			++brackets_open;
		else if (src[i] == '}')
			--brackets_open;
		ret += src[i];
	}
	if (!brackets_open)
	{
		--i;
		return (ret);
	}
	else
		throw std::invalid_argument("[Error] incorrect bracket syntax.");
}

std::string	nextWord( std::string const & src, size_t & i )
{
	std::string	ret;

	for (; isspace(src[i]) && src[i] ; ++i) {}
	//std::cout << "start with " << src[i] << std::endl;
	for (; !isspace(src[i]) && src[i] != ';' && src[i] ; ++i)
	{
		if (src[i] == '{')
			return (parceBrackets(src, i, ret));
		else
			ret += src[i];
	}

	return (ret);
}

std::vector< std::vector<std::string> >	sortInVec( std::string const & src )
{
	std::vector< std::vector<std::string> >			ret;
	std::vector<std::string>						tmp;
	std::string	nxt;
	size_t	i = 0;

	while (src[i])
	{
		nxt = nextWord(src, i);
		if (nxt.size())
			tmp.push_back(nxt);
		if ((src[i] == ';' || src[i] == '}' || !src[i]) && tmp.size())
		{
			ret.push_back(tmp);
			tmp.clear();
			if (src[i])
				++i;
		}
	}

	// print content (to delete)
	/*std::cout << "start print:" << std::endl;
	std::vector< std::vector<std::string> >::const_iterator	it, end;
	std::vector<std::string>::const_iterator	sit, send;
	int j = 0;

	end = ret.end();
	for (it = ret.begin() ; it != end ; ++it)
	{
		std::cout << ++j << std::endl;
		send = (*it).end();
		for (sit = (*it).begin() ; sit != send ; ++sit)
			std::cout << "-" << *sit << "\n";
		std::cout << "\n";
	}*/
	return (ret);
}