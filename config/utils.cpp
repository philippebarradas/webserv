/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 19:02:55 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/07 19:07:57 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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
			--i;
		}
		else
			ret += src[i];
	}

	return (ret);
}

std::vector< std::vector<std::string> >	sortInVec( std::string const & src )
{
	std::vector< std::vector<std::string> >	ret;
	std::vector<std::string>				tmp;
	std::string	nxt;
	size_t	i = 0;

	while (src[i])
	{
		nxt = nextWord(src, i);
		tmp.push_back(nxt);
		if (src[i] == ';' || src[i] == '}')
		{
			ret.push_back(tmp);
			tmp.clear();
			++i;
		}
	}

	// print content (to delete)
	std::vector< std::vector<std::string> >::iterator	it, end;
	std::vector<std::string>	sit, send;
	end = ret.end();
	for (it = ret.begin() ; it != end ; ++it)
	{
		send = (*it).end();
		for (sit = (*it).begin() ; sit != send ; ++sit)
			std::cout << *sit << "\n";
		std::cout << "\n";
	}


	return (ret);
}
