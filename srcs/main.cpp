/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2022/02/25 16:47:20 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/Config.hpp"
#include "Cgi/Cgi.hpp"

void signal_callback_handler( int ssignum )
{
	throw SignalStop();
}

int	main( int ac, char *av[] )
{
	if (ac != 2)
		return (1);

	signal(SIGINT, signal_callback_handler);

	Config		conf;
	Engine		serv;
	std::vector<Server> vec_serv;

	try
	{
		conf = Config(av[1]);
		serv = Engine(conf.getConfig());
	}
	catch( SignalStop const & e )
	{
		static_cast<void>(e);
		//std::cerr << e.what() << std::endl;

	}
	catch( std::exception const & e )
	{
		//std::cerr << "An error has been found on the config file:" << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
