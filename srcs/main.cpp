/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/25 16:56:31 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/Config.hpp"
#include "Cgi/Cgi.hpp"

void signal_to_exit( int ssignum ) { throw SignalStop(); }

int	main( int ac, char *av[] )
{
	if (ac != 2)
		return (1);


	Config		conf;
	Engine		serv;
	std::vector<Server> vec_serv;

	try
	{
		conf = Config(av[1]);
	}
	catch( const std::exception& e )
	{
		std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
	}


	try
	{
		signal(SIGINT, signal_to_exit);
		serv = Engine(conf.getConfig());
	}
	catch( SignalStop const & e )
	{
		static_cast<void>(e);
	}
	catch( std::exception const & e )
	{
		//std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
