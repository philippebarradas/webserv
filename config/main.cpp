/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/20 09:17:46 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int	main( int ac, char *av[] )
{
	if (ac != 2)
		return (1);

	Config		conf;

	try
	{
		conf = Config(av[1]);
	}
	catch( std::exception const & e )
	{
		std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	try
	{
		std::vector<Server>::const_iterator it;

		for (it = conf.getConfig().begin() ; it != conf.getConfig().end() ; ++it)
		{
			LaunchServ serv(*it);
		}
	}
	catch( std::exception const & e )
	{
		//std::cerr << "An error has been found on the launching server:" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	std::vector<Server>::const_iterator it;

	std::cout << conf.getConfig().size() << std::endl;
	std::cout << conf << std::endl;

	return (0);
}
