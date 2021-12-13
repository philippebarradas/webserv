/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/13 20:28:46 by tsannie          ###   ########.fr       */
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
		conf = Config((std::string(av[1])));
	}
	catch( std::exception const & e )
	{
		std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::vector<Server>::const_iterator it;

	std::cout << "---------------" << std::endl;
	//it = conf.getConfig().begin();

	std::cout << "here:" << *(it = conf.getConfig().begin()) << std::endl;

	std::cout << conf.getConfig().size() << std::endl;
	//std::cout << conf << std::endl;

	return (0);
}
