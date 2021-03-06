/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:54:38 by tsannie           #+#    #+#             */
/*   Updated: 2022/03/04 00:11:14 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/Config.hpp"
#include "Cgi/Cgi.hpp"

void signal_to_exit( int ssignum )
{
	static_cast<void>(ssignum);
	throw SignalStop();
}

bool	init_conf( Config & conf, char *path_conf_file )
{
	try
	{
		conf = Config(path_conf_file);
	}
	catch( const std::exception& e )
	{
		std::cerr << "An error has been found in the execution of webserv:\n"
			<< e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool	start_engine( Config const & conf )
{
	try
	{
		Engine(conf.getConfig());
	}
	catch( std::exception const & e )
	{
		std::cerr << "An error has been found on the config file:\n"
			<< e.what() << std::endl;
		return (false);
	}
	return (true);
}

int	main( int ac, char *av[] )
{
	signal(SIGINT, signal_to_exit);
	if (ac != 2)
	{
		std::cerr << "Error\nUsage: ./webserv \"[config_file]\"" << std::endl;
		return (1);
	}

	Config		conf;
	if (!init_conf(conf, av[1]))
		return (1);

	if (!start_engine(conf))
		return (1);

	return (0);
}
