/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/24 15:50:41 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/Config.hpp"
#include "Cgi/Cgi.hpp"

std::string	int_to_string(int x)
{
	return (static_cast<std::ostringstream*>( &(std::ostringstream() << x))->str());
}

int	main( int ac, char *av[] )
{
	if (ac != 2)
		return (1);

	Config		conf;
	std::vector<Server> vec_serv;

	try
	{
		conf = Config(av[1]);
		//std::cout << conf << std::endl;
		vec_serv = conf.getConfig();

		Engine serv(vec_serv);
		//Cgi		obj_cgi(vec_serv.front());
	}
	catch( std::exception const & e )
	{
		std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}
	//std::cout << conf.getConfig().size() << std::endl;
	//std::cout << conf << std::endl;

	return (0);
}
